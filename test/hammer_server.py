import threading
import requests
from requests.auth import HTTPDigestAuth
import os

NUM_OF_REQUESTS = 250
NUM_OF_THREADS = 2
URL = "https://172.18.0.2:8081/helloworld"
CERT_PATH = "/usr/local/share/ca-certificates/smoothstack_client.crt"

def hammer():
    for i in range(NUM_OF_REQUESTS):
        request = requests.get(URL, auth=HTTPDigestAuth("calvin", "mypass"), verify=CERT_PATH)
        if request.status_code != 200:
            print(f"Call-{i} {threading.currentThread().getName()}: {request.status_code}")

        #os.system("curl https://172.18.0.2:8081/helloworld \n")


if __name__ == "__main__":
    threads = []
    for i in range(NUM_OF_THREADS):
        thread = threading.Thread(target=hammer)
        thread.start()
        threads.append(thread)
    for thread in threads:
        thread.join()