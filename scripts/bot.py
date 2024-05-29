import requests
import concurrent.futures
import time
import random
import threading
from datetime import datetime, timedelta

def get_table(n_table):
    try:
        url = f"https://192.168.1.66:18080/api/products/table/{str(n_table)}"
        print("URL:", url)
        
        response = requests.get(url, verify=False, timeout=10)
        print("Response received")
        
        response.raise_for_status()
        print("Response status is OK")
        
        try:
            data = response.json()
            print("JSON data parsed successfully")
        except ValueError as e:
            print(f"Failed to parse JSON: {e}")
            return False, 0
        
        print("Response JSON data:", data)
        
        is_empty = data.get("n_table") == 0
        print("Is table empty:", is_empty)
        
        n_clients = data.get("n_clients", 0)
        
        return (False if is_empty else True), n_clients
    except requests.exceptions.RequestException as e:
        print(f"An error occurred while fetching table: {e}")
        return False, 0

def get_products(api_url):
    try:
        print("PRODUCTSSSSSSSSS1")
        response = requests.get(api_url, verify=False)
        print("PRODUCTSSSSSSSSS2")
        response.raise_for_status()
        data = response.json()
        
        products = []
        for item in data:
            print("pr")
            product = {
                "id": item.get("id"),
                "name": item.get("name"),
                "page": item.get("page"),
                "menu": item.get("menu"),
                "price": str(item.get("price")),
                "details": ""
            }
            products.append(product)
            
        return products
    except requests.exceptions.RequestException as e:
        print(f"An error occurred while fetching products: {e}")
        return []

def send_post_request(product_list):
    try:
        print("AAAAHHH1")
        
        url = "https://192.168.1.66:18080/order"
        num_products = random.randint(1, len(product_list))
        print("AAAAHHH2")
        
        added_products = random.sample(product_list, num_products)
        print("AAAAHHH3")
        
        for product in added_products:
            print("AAAAHHH4")
            
            product['times'] = random.randint(1, 10)
        
        payload = {
            "order": [],
            "added": added_products,
            "modified": [],
            "deleted": [],
            "n_table": random.randint(1, 100),
            "n_clients": random.randint(1, 10),
            "employee": "Adrian Labaro H. T."
        }
        print("AAAAHHH5")
        
        response = requests.post(url, json=payload, verify=False, timeout=10)
        print("AAAAHHH6")
        
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

def send_payTable_request(n_table, n_clients, product_list):
    try:
        print("AVAILABLEEEEEEE")
        
        url = "https://192.168.1.66:18080/payTable"
        num_products = random.randint(1, len(product_list))
        added_products = random.sample(product_list, num_products)
        bill = 0.00
        
        for product in added_products:
            print(product['names'])
            product['times'] = random.randint(1, 10)
            bill += float(product['price']) * product['times']
        
        payload = {
            "id": 0,
            "n_table": n_table,
            "n_clients": n_clients,
            "bill": bill,
            "paid": bill + random.uniform(0.00, 10.00),
            "discount": 0,
            "method": "Card" if random.randint(0,1) else "Cash",
            "employee": "Adrian Labaro H. T.",
            "date": random_date(datetime(2024, 1, 1), datetime(2024, 12, 31, 23, 59, 59)).strftime('%Y-%m-%d %H:%M:%S')
        }
        print("alele1")
        response = requests.post(url, json=payload, verify=False, timeout=10)
        print("alele2")
        
        response.raise_for_status()
        print("alele3")
        
        print(f"Response from {url}: {response.status_code}")
        return response.status_code == 200
    except requests.exceptions.RequestException as e:
        print(f"An error occurred: {e}")
        return False

def main(api_url, num_threads=1000):
    successful_requests = 0
    total_requests = 0
    stop_event = threading.Event()

    def check_for_stop():
        input("Press Enter to stop...\n")
        stop_event.set()

    threading.Thread(target=check_for_stop).start()

    print("ENTERING PRODUCTS")
    products = get_products(api_url)
    if not products:
        print("No products fetched. Exiting.")
        return

    while not stop_event.is_set():
        with concurrent.futures.ThreadPoolExecutor(max_workers=num_threads) as executor:      
            futures = []
            n_table = random.randint(1, 100)
            table_available, n_clients = get_table(n_table)
            print("LOOOL")

            if table_available:
                futures.append(executor.submit(send_payTable_request, n_table, n_clients, products))
            else:
                futures.append(executor.submit(send_post_request, products))

            for future in concurrent.futures.as_completed(futures):        
                try:
                    if future.result():
                        successful_requests += 1
                    total_requests += 1
                except Exception as e:
                    print(f"Exception: {e}")

        # Random sleep time between 0.1s and 1s with a precision of 0.1s
        sleep_time = round(random.uniform(0.1, 1.0), 1)
        time.sleep(sleep_time)

    print(f"\nTotal successful requests: {successful_requests}")

if __name__ == "__main__":
    api_url = "https://192.168.1.66:18080/api/products/all"
    main(api_url)
