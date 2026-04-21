import nox
import subprocess
import time
import re
import threading


@nox.session()
def test_uart_api_socat(session):

    session.log("STEP 1: Building project")
    session.run("make", external=True)

    session.log("STEP 2: Creating PTY pair using socat")

    socat = subprocess.Popen(
        ["socat", "-d", "-d", "pty,raw,echo=0", "pty,raw,echo=0"],
        stderr=subprocess.PIPE,
        text=True,
    )

    ptys = []

    while len(ptys) < 2:
        line = socat.stderr.readline()
        session.log(f"socat stderr: {line.strip()}")

        match = re.search(r"/dev/pts/\d+", line)
        if match:
            ptys.append(match.group())

    pty1, pty2 = ptys

    session.log(f"STEP 3: PTY1 assigned -> {pty1}")
    session.log(f"STEP 4: PTY2 assigned -> {pty2}")

    session.log("STEP 5: Starting responder")

    responder = subprocess.Popen(
        ["socat", pty2, "-"],
        stdout=subprocess.PIPE,
        stdin=subprocess.PIPE,
        text=True,
        bufsize=1,
    )

    received_uart_message = False
    reply_sent = False

    def responder_logic():
        nonlocal received_uart_message
        nonlocal reply_sent

        session.log("STEP 6: Waiting for UART TX")

        while True:

            line = responder.stdout.readline()

            if not line:
                break

            session.log(f"Responder received: {line.strip()}")

            if "UART test message" in line:
                received_uart_message = True

                session.log("STEP 7: Sending HELLO BACK")

                responder.stdin.write("HELLO BACK\n")
                responder.stdin.flush()

                reply_sent = True
                break

    thread = threading.Thread(target=responder_logic)
    thread.start()

    session.log("STEP 8: Launching uart.o (stdout unbuffered)")

    uart_proc = subprocess.Popen(
        ["stdbuf", "-o0", "./uart.o", pty1, "115200", "8", "N", "1", "3"],
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
    )

    session.log("STEP 9: Waiting for RX loop to print response")
    time.sleep(7)

    session.log("STEP 10: Terminating uart.o")
    uart_proc.terminate()

    try:
        stdout, stderr = uart_proc.communicate(timeout=2)
    except subprocess.TimeoutExpired:
        uart_proc.kill()
        stdout, stderr = uart_proc.communicate()

    responder.kill()
    socat.kill()

    session.log("STEP 11: UART stdout captured below")
    session.log(stdout)

    if not received_uart_message:
        raise RuntimeError("FAILURE: UART TX not detected")

    if not reply_sent:
        raise RuntimeError("FAILURE: HELLO BACK never sent")

    if "HELLO BACK" not in stdout:
        raise RuntimeError("FAILURE: UART RX not detected")

    session.log("STEP 12: UART automated test passed")