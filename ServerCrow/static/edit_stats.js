let selectedFilterElement = document.getElementById("allOrders");
let selectedFilter = selectedFilterElement.textContent;

retrieveOrders();
async function retrieveOrders() {
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
        console.log("retrieveOrders ", jsonData);

        populateTable(jsonData);
    }
    catch (error) {
        console.error('Error:', error);
        throw error;
    }
}

async function retrieveOrdersByDate(date, mode) {
    const baseUrl = window.location.href.replace(window.location.pathname, '');
    const fetchPath = '/api/orders/' + mode + "/" + date;
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

        populateTable(jsonData);
    }
    catch (error) {
        console.error('Error:', error);
        throw error;
    }
}

async function retrieveOrdersByEmployee(employee) {
    const baseUrl = window.location.href.replace(window.location.pathname, '');
    const fetchPath = '/api/orders/employee/' + employee;
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

        populateTable(jsonData);
    }
    catch (error) {
        console.error('Error:', error);
        throw error;
    }
}

async function populateTable(data) {
    let orders = await data;
    const statsTable = document.getElementById("ordersTable").children[1];
    statsTable.innerHTML = "";
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

function selectFilter(clickedElement) {
    selectedFilterElement.style.backgroundColor = "rgb(9, 43, 92)";

    selectedFilterElement = clickedElement;
    selectedFilter = clickedElement.textContent;

    selectedFilterElement.style.backgroundColor = "rgb(28, 89, 176)";
}

async function searchOrders() {
    let input = document.getElementById("ordersFilterInput").value;
    let res;
    console.log("search ", selectedFilter, input);
    switch (selectedFilter) {
        case "All":
            res = await retrieveOrders();
            break;
        case "Id":

            break;
        case "Date":
            console.log("date");
            res = await retrieveOrdersByDate(input, "date");
            console.log(res);
            break;
        case "Week":
            retrieveOrdersByDate(input, "week");
            break;
        case "Month":
            retrieveOrdersByDate(input, "month");
            break;
        case "Year":
            retrieveOrdersByDate(input, "year");
            break;
        case "N. table":
            //retrieveOrdersByNTable(input, n_table);
            break;
        case "Method":
            //retrieveOrdersMethod(input, method);
            break;
        case "Employee":
            res = await retrieveOrdersByEmployee(input);
            break;
        default:
            console.log("Error selecting filter");
            break;
    }

    console.log(res);
}