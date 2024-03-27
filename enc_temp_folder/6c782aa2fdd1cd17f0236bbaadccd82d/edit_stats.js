export async function retrieveOrders() {
    const baseUrl = window.location.href.replace(window.location.pathname, '');
    const fetchPath = '/api/orders';
    const url = baseUrl + fetchPath;
    console.log(url);

    try {
        const response = await fetch(url, {
            method: 'GET',
            headers: {
                'Content-Type': 'application/json',
            }
        });
        if (!response.ok) {
            throw new Error('Network response was not ok');
        }
        const jsonData = await response.json();
        console.log("jsondata ", jsonData);
        return jsonData;
    } catch (error) {
        console.error('Error:', error);
        throw error;
    }
}

export async function populateTable() {
    const statsTable = document.getElementById("ordersTable");
    console.log(statsTable);
    const orders = await retrieveOrders();
    console.log("orders ", orders);

    let i = 0;
    for (let order of orders) {
        const newRow = document.createElement('tr');

        const id = document.createElement('td');
        id.textContent = order["id"];

        const date = document.createElement('td');
        date.textContent = order["date"];

        const n_table = document.createElement('td');
        n_table.textContent = order["n_table"];

        const n_clients = document.createElement('td');
        n_clients.textContent = order["n_clients"];

        const bill = document.createElement('td');
        bill.textContent = order["bill"].toFixed(2);

        let paid = document.createElement('td');
        paid.textContent = order["paid"].toFixed(2);
        if (parseFloat(bill.textContent) < parseFloat(paid.textContent)) paid.style.color = "green";
        if (parseFloat(bill.textContent) > parseFloat(paid.textContent)) paid.className = "decrement";

        const discount = document.createElement('td');
        discount.textContent = "-" + order["discount"] + "%";

        const method = document.createElement('td');
        method.textContent = order["method"];

        const employee = document.createElement('td');
        employee.textContent = order["employee"];

        newRow.appendChild(id);
        newRow.appendChild(date);
        newRow.appendChild(n_table);
        newRow.appendChild(n_clients);
        newRow.appendChild(bill);
        newRow.appendChild(paid);
        newRow.appendChild(discount);
        newRow.appendChild(method);
        newRow.appendChild(employee);

        if (statsTable) statsTable.appendChild(newRow);

        i++;
    }
}