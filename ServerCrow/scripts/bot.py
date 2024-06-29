from itertools import product
import requests
import concurrent.futures
import time
import random
import threading
from datetime import datetime, timedelta

def get_table(n_table):
    try:
        url = f"https://192.168.1.66:18080/api/products/table/{str(n_table)}"
        
        response = requests.get(url, verify=False, timeout=TIMEOUT)     
        response.raise_for_status()
        
        try:
            data = response.json()
        except ValueError as e:
            print(f"Failed to parse JSON: {e}")
            return False, 0
        
        print("N TABLE ", n_table)
        print("Response JSON data:", data)
        
        is_empty = data.get("bill") == 0.0
        print("Is table empty:", is_empty)
        
        n_clients = data.get("n_clients", 0)

        json = data
        
        return (False if is_empty else True), json
    except requests.exceptions.RequestException as e:
        print(f"An error occurred while fetching table: {e}")
        return False, 0

def get_products(api_url):
    try:
        print("DEPLOYABLE11111: ")
        
        response = requests.get(api_url, verify=False)
        response.raise_for_status()
        data = response.json()
        
        print("DEPLOYABLE: ")
        products = []
        for item in data:
            print(item.get("deployable"))
            if item.get("deployable") != 0:
                product = {
                    "id": item.get("id"),
                    "name": item.get("name"),
                    "page": item.get("page"),
                    "price": str(item.get("price")),
                    "details": ""
                }
                products.append(product)
            
        return products
    except requests.exceptions.RequestException as e:
        print(f"An error occurred while fetching products: {e}")
        return []

def send_post_request(product_list, n_table):
    try:     
        url = "https://192.168.1.66:18080/order"
        num_products = random.randint(1, 5)

        added_products = random.sample(product_list, num_products)
        
        for product in added_products:     
            product['times'] = random.randint(1, 5)
        
        payload = {
            "order": [],
            "added": added_products,
            "modified": [],
            "deleted": [],
            "n_table": n_table,
            "n_clients": random.randint(1, 10),
            "employee": "Adrian Labaro H. T."
        }
        
        print("ADDED PRODUCTS")
        print(added_products)
        response = requests.post(url, json=payload, verify=False, timeout=TIMEOUT)
        
        response.raise_for_status()
        print(f"Response from {url}: {response.status_code}")
        return response.status_code == 200
    except requests.exceptions.RequestException as e:
        print(f"An error occurred: {e}")
        return False
  
def random_date(start, end):
    """Generate a random datetime between `start` and `end`."""
    delta = end - start
    int_delta = (delta.days * 24 * 60 * 60) + delta.seconds
    random_second = random.randrange(int_delta)
    return start + timedelta(seconds=random_second)

def send_payTable_request(json):
    try:   
        url = "https://192.168.1.66:18080/payTable"
        bill = 0.00
        
        payload = {
            "id": 0,
            "n_table": json["n_table"],
            "n_clients": json["n_clients"],
            "bill": json["bill"],
            "paid": json["bill"] + (random.randint(0, 1) * random.uniform(1, json["bill"]*0.3)),
            "discount": json["discount"],
            "method": "Card" if random.randint(0,1) else "Cash",
            "employee": "Adrian Labaro H. T.",
            "date": random_date(datetime(2024, 1, 1), datetime(2024, 12, 31, 23, 59, 59)).strftime('%Y-%m-%d %H:%M:%S')
        }
        
        print("PAIDDDDD")
        print(payload["paid"])

        response = requests.post(url, json=payload, verify=False, timeout=TIMEOUT)       
        response.raise_for_status()
        
        """print(f"Response from {url}: {response.status_code}")"""
        return response.status_code == 200
    except requests.exceptions.RequestException as e:
        print(f"An error occurred: {e}")
        return False

def main(api_url):   
    successful_requests = 0
    total_requests = 0
    stop_event = threading.Event()

    def check_for_stop():
        input("Press Enter to stop...\n")
        stop_event.set()

    threading.Thread(target=check_for_stop).start()

    products = get_products(api_url)
    if not products:
        print("No products fetched. Exiting.")
        return

    while not stop_event.is_set():
        with concurrent.futures.ThreadPoolExecutor(max_workers=NUM_THREADS) as executor:      
            futures = []
            n_table = random.randint(1, 100)
            table_available, json = get_table(n_table)

            if table_available:
                print("PAYING TABLE")
                futures.append(executor.submit(send_payTable_request, json))
            else:
                print("ADDING PRODUCTS")
                futures.append(executor.submit(send_post_request, products, n_table))

            for future in concurrent.futures.as_completed(futures):        
                try:
                    if future.result():
                        successful_requests += 1
                    total_requests += 1
                except Exception as e:
                    print(f"Exception: {e}")

        # Random sleep time between 0.1s and 1s with a precision of 0.1s
        sleep_time = round(random.uniform(0.1, SLEEP_TIME), 1)
        time.sleep(sleep_time)

    print(f"\nTotal successful requests: {successful_requests}")

if __name__ == "__main__":
    api_url = "https://192.168.1.66:18080/api/products/all"
    TIMEOUT = 999
    SLEEP_TIME = 0.5
    NUM_THREADS = 1000
    main(api_url)
