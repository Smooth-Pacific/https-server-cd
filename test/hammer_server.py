import grequests
import threading
import requests
from requests.auth import HTTPDigestAuth

# pip3 install grequests

# This program will get some 401's because requests is not thread safe with digest authentication enabled

NUM_OF_REQUESTS = 250
NUM_OF_THREADS = 8
URL = "https://172.18.0.2:8081/helloworld"
CERT_PATH = "/usr/local/share/ca-certificates/smoothstack_client.crt"
BULK_SEND = False       # if set true, will send many requests at once using grequests

lock = threading.Lock()

def hammer():
    if BULK_SEND:
        # create session
        s = requests.Session()
        s.auth = HTTPDigestAuth("calvin", "mypass")
        s.verify = CERT_PATH

        # creates a set of unsent requests with a single session
        rs = (grequests.get(URL, session=s) for _ in range(NUM_OF_REQUESTS))

        # send all the requests at once
        for response in grequests.imap(rs, 10):
            print(f"{threading.currentThread().getName()} {response}")

    else:
        s = requests.Session()
        s.auth = HTTPDigestAuth("calvin", "mypass")
        s.verify = CERT_PATH

        for i in range(NUM_OF_REQUESTS):
            r = s.get(URL)
            print(f"{threading.currentThread().getName()} {r.status_code} {r.content}")

if __name__ == "__main__":
    threads = []
    for i in range(NUM_OF_THREADS):
        thread = threading.Thread(target=hammer)
        thread.start()
        threads.append(thread)

    for thread in threads:
        thread.join()