let selectedFilterElement = document.getElementById("allOrders");
let selectedFilter = selectedFilterElement.textContent;
let selectedRow;
let currentOrders;

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

async function retrieveOrdersByNTable(n_table) {
    const baseUrl = window.location.href.replace(window.location.pathname, '');
    const fetchPath = '/api/orders/n_table/' + n_table;
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

async function retrieveOrdersByMethod(method) {
    const baseUrl = window.location.href.replace(window.location.pathname, '');
    const fetchPath = '/api/orders/method/' + method;
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

function selectRow(clickedRow) {
    console.log(selectedRow);

    if (!selectedRow) { // If it is the first clicked element
        selectedRow = clickedRow;

        clickedRow.style.backgroundColor = "rgb(28, 89, 176)";
        clickedRow.style.color = "white";
    }
    else {
        let selectedRowId = selectedRow.children[0].textContent;
        let bgColor = (parseInt(selectedRowId) % 2) !== 0 ? "#d6d6d6" : "white";

        selectedRow.style.backgroundColor = bgColor;
        selectedRow.style.color = "black";

        if (selectedRow !== clickedRow) { // If the selectedRow is clicked
            selectedRow = clickedRow;

            clickedRow.style.backgroundColor = "rgb(28, 89, 176)";
            clickedRow.style.color = "white";
        }
        else {
            selectedRow = null;
            let productList = document.getElementById("productList");
            productList.innerHTML = "";

            clickedRow.style.backgroundColor = bgColor;
            clickedRow.style.color = "black";
        }
    }

    if (selectedRow !== null) {
        let order_id = clickedRow.children[0].textContent;
        getProductsFromOrder(order_id);
    }
}

function getProductsFromOrder(order_id) {
    let res;
    let order = currentOrders.find(ord => ord.id == order_id);
    let orderProducts = order["products"];
    let productList = document.getElementById("productList");

    orderProducts.sort((a, b) => a.name.localeCompare(b.name));
    productList.innerHTML = "";

    for (let product of orderProducts) {
        let prod = document.createElement('tr');
        prod.className = "productListRow";

        let prodNameElement = document.createElement('td');
        prodNameElement.class = "prodName";
        prodNameElement.textContent = product["name"];

        let prodPriceElement = document.createElement('td');
        prodPriceElement.class = "prodPrice";
        prodPriceElement.textContent = product["price"].toFixed(2);

        let prodAmountElement = document.createElement('td');
        prodAmountElement.class = "prodAmount";
        prodAmountElement.textContent = product["amount"];

        let prodTotalElement = document.createElement('td');
        prodTotalElement.class = "prodTotal";
        prodTotalElement.textContent = (parseFloat(prodPriceElement.textContent) * parseFloat(prodAmountElement.textContent)).toFixed(2);

        prod.appendChild(prodNameElement);
        prod.appendChild(prodPriceElement);
        prod.appendChild(prodAmountElement);
        prod.appendChild(prodTotalElement);

        productList.appendChild(prod);
    }
}

async function populateTable(data) {
    let orders = await data;
    const statsTable = document.getElementById("ordersTable").children[1];
    statsTable.innerHTML = "";
    console.log("orders ", orders);

    currentOrders = orders;
    console.log("currentOrders ", currentOrders);
    let i = 0;
    for (let order of orders) {
        let newRow = document.createElement('tr');
        newRow.onclick = function () { selectRow(newRow) };

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

        let diff = document.createElement('td');
        diff.textContent = (parseFloat(paid.textContent) - parseFloat(bill.textContent)).toFixed(2);
        if (parseFloat(diff.textContent) > 0) {
            diff.textContent = "+" + diff.textContent;
            diff.style.color = "green";
        }
        else if (parseFloat(diff.textContent < 0)) {
            diff.textContent = "-" + diff.textContent
            diff.style.color = "red";
        }
        else {
            diff.textContent = "";
        }

        const discount = document.createElement('td');
        discount.textContent = "-" + order["discount"].toFixed(2) + "%";
        if (discount.textContent == "-0.00%") discount.textContent = "";

        const method = document.createElement('td');
        method.textContent = order["method"];

        const employee = document.createElement('td');
        employee.textContent = order["employee"];

        //console.log(order["products"]);
        //console.log(products.textContent);

        newRow.appendChild(id);
        newRow.appendChild(date);
        newRow.appendChild(n_table);
        newRow.appendChild(n_clients);
        newRow.appendChild(bill);
        newRow.appendChild(paid);
        newRow.appendChild(diff);
        newRow.appendChild(discount);
        newRow.appendChild(method);
        newRow.appendChild(employee);

        if (parseInt(id.textContent) % 2 !== 0) newRow.style.backgroundColor = "#d6d6d6";

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
            retrieveOrdersByNTable(input);
            break;
        case "Method":
            retrieveOrdersByMethod(input);
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