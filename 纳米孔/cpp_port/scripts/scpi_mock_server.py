#!/usr/bin/env python3
import socket
import threading
import argparse

RESP_IDN = "KEITHLEY INSTRUMENTS INC.,MODEL 2450,04123456,1.0.0\n"

class ScpiMockHandler(threading.Thread):
    def __init__(self, conn, addr):
        super().__init__(daemon=True)
        self.conn = conn
        self.addr = addr
        self.voltage = 0.0
        self.output_on = False

    def run(self):
        try:
            with self.conn:
                buf = b""
                while True:
                    data = self.conn.recv(4096)
                    if not data:
                        break
                    buf += data
                    while b"\n" in buf:
                        line, buf = buf.split(b"\n", 1)
                        cmd = line.decode(errors="ignore").strip()
                        resp = self.handle(cmd)
                        if resp is not None:
                            self.conn.sendall(resp)
        except Exception:
            pass

    def handle(self, cmd: str):
        c = cmd.upper()
        # Basic SCPI
        if c == "*IDN?":
            return RESP_IDN.encode()
        if c.startswith("SOUR:VOLT "):
            try:
                self.voltage = float(cmd.split(" ", 1)[1])
            except Exception:
                self.voltage = 0.0
            return None
        if c == "SOUR:VOLT?":
            return f"{self.voltage}\n".encode()
        if c == "MEAS:CURR?":
            # Tiny fake current proportional to voltage
            curr = self.voltage * 1e-9 if self.output_on else 0.0
            return f"{curr}\n".encode()
        if c == "OUTP ON":
            self.output_on = True
            return None
        if c == "OUTP OFF":
            self.output_on = False
            return None
        if c == "SYST:ERR?":
            return b"0,\"No error\"\n"
        if c in ("*RST", "*CLS"):
            self.voltage = 0.0
            self.output_on = False
            return None
        # Default OK
        return b"OK\n"


def run_server(host: str, port: int):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        s.bind((host, port))
        s.listen(5)
        print(f"SCPI mock server listening on {host}:{port}")
        while True:
            conn, addr = s.accept()
            ScpiMockHandler(conn, addr).start()


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--host", default="127.0.0.1")
    ap.add_argument("--port", type=int, default=5025)
    args = ap.parse_args()
    run_server(args.host, args.port)


if __name__ == "__main__":
    main()