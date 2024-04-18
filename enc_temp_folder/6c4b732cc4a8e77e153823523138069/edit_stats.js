let selectedOrderFilterElement = document.getElementById("allOrders");
let selectedOrderFilter = selectedOrderFilterElement.textContent;
let selectedProductFilterElement = document.getElementById("allProducts");
let selectedProductFilter = selectedProductFilterElement.textContent;
let selectedRow;
let currentOrders;
let currentProducts;

retrieveOrders();
retrieveProducts();
async function retrieveOrders() {
    const baseUrl = window.location.href.replace(window.location.pathname, '');
    const fetchPath = '/api/orders';
    const url = baseUrl + fetchPath;

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

async function retrieveOrderById(id) {
    const baseUrl = window.location.href.replace(window.location.pathname, '');
    const fetchPath = '/api/orders/id/' + id;
    const url = baseUrl + fetchPath;

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

async function retrieveOrdersByDate(date, mode) {
    const baseUrl = window.location.href.replace(window.location.pathname, '');
    const fetchPath = '/api/orders/' + mode + "/" + date;
    const url = baseUrl + fetchPath;

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
    if (!selectedRow) { // If it is the first clicked element
        selectedRow = clickedRow;

        clickedRow.style.backgroundColor = "rgb(28, 89, 176)";
        clickedRow.style.color = "white";
    }
    else {
        let selectedRowId = selectedRow.children[0].textContent;
        let bgColor = (parseInt(selectedRowId) % 2) !== 0 ? "rgb(237, 237, 237)" : "white";

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

            const productsMenuTotalAmount = document.getElementById("productsMenuTotalAmount");
            const productsMenuTotalBill = document.getElementById("productsMenuTotalBill");
            productsMenuTotalAmount.textContent = "-";
            productsMenuTotalBill.textContent = "-";
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
    const productsMenuTotalAmount = document.getElementById("productsMenuTotalAmount")
    let totalAmount = 0;
    let totalBill = 0;
    const productsMenuTotalBill = document.getElementById("productsMenuTotalBill");

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
        totalAmount += parseInt(prodAmountElement.textContent);

        let prodTotalElement = document.createElement('td');
        prodTotalElement.class = "prodTotal";
        prodTotalElement.textContent = (parseFloat(prodPriceElement.textContent) * parseFloat(prodAmountElement.textContent)).toFixed(2);
        totalBill += parseFloat(prodTotalElement.textContent);

        prod.appendChild(prodNameElement);
        prod.appendChild(prodPriceElement);
        prod.appendChild(prodAmountElement);
        prod.appendChild(prodTotalElement);

        productList.appendChild(prod);
    }

    productsMenuTotalAmount.textContent = totalAmount.toString();
    productsMenuTotalBill.textContent = totalBill.toFixed(2);
}

async function populateTable(data) {
    let orders = await data;
    const statsTable = document.getElementById("ordersTable").children[1];
    statsTable.innerHTML = "";

    currentOrders = orders;
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

        if (parseFloat(diff.textContent) > 0.0) {
            diff.textContent = "+" + diff.textContent;
            diff.style.color = "green";
        }
        else if (parseFloat(diff.textContent) < 0.0) {
            diff.textContent = diff.textContent
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

        if (i % 2 === 0) newRow.style.backgroundColor = "rgb(237, 237, 237)";

        if (statsTable) statsTable.appendChild(newRow);

        i++;
    }
}

function selectFilter(clickedElement) {
    if (clickedElement.parentNode.id === "ordersFilters") {
        selectedOrderFilterElement.style.backgroundColor = "rgb(9, 43, 92)";

        selectedOrderFilterElement = clickedElement;
        selectedOrderFilter = clickedElement.textContent;

        selectedOrderFilterElement.style.backgroundColor = "rgb(28, 89, 176)";
    }
    else if (clickedElement.parentNode.id === "productsFilters") {
        selectedProductFilterElement.style.backgroundColor = "rgb(9, 43, 92)";

        selectedProductFilterElement = clickedElement;
        selectedProductFilter = clickedElement.textContent;

        selectedProductFilterElement.style.backgroundColor = "rgb(28, 89, 176)";
    }
}

async function searchOrders() {
    let input = document.getElementById("ordersFilterInput").value;
    let res;

    switch (selectedOrderFilter) {
        case "All":
            res = await retrieveOrders();
            break;
        case "Id":
            res = await retrieveOrderById(input);
            break;
        case "Date":
            res = await retrieveOrdersByDate(input, "date");
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
}

async function retrieveProducts() {
    const baseUrl = window.location.href.replace(window.location.pathname, '');
    const fetchPath = '/api/products';
    const url = baseUrl + fetchPath;

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

        populateProductsTable(jsonData);
    }
    catch (error) {
        console.error('Error:', error);
        throw error;
    }
}

async function retrieveProductById(id) {
    const baseUrl = window.location.href.replace(window.location.pathname, '');
    const fetchPath = '/api/products/id/' + id;
    const url = baseUrl + fetchPath;

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

        populateProductsTable(jsonData);
    }
    catch (error) {
        console.error('Error:', error);
        throw error;
    }
}

async function retrieveProductByName(name) {
    const baseUrl = window.location.href.replace(window.location.pathname, '');
    const fetchPath = '/api/products/name/' + name;
    const url = baseUrl + fetchPath;

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

        populateProductsTable(jsonData);
    }
    catch (error) {
        console.error('Error:', error);
        throw error;
    }
}

async function retrieveProductsByDate(date, mode) {
    const baseUrl = window.location.href.replace(window.location.pathname, '');
    const fetchPath = '/api/products/' + mode + "/" + date;
    const url = baseUrl + fetchPath;

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

        populateProductsTable(jsonData);
    }
    catch (error) {
        console.error('Error:', error);
        throw error;
    }
}

async function populateProductsTable(jsonData) {
    let products = await jsonData;
    const statsTable = document.getElementById("productsTable").children[1];
    statsTable.innerHTML = "";

    if (products) {
        products.sort((a, b) => b.sold - a.sold);

        currentProducts = products;

        let i = 0;
        for (let product of products) {
            let newRow = document.createElement('tr');

            const id = document.createElement('td');
            id.textContent = product["id"];

            const name = document.createElement('td');
            name.textContent = product["name"];

            const price = document.createElement('td');
            price.textContent = product["price"].toFixed(2);

            const sold = document.createElement('td');
            sold.textContent = product["sold"];

            const percent = document.createElement('td');
            percent.textContent = product["percent"].toFixed(2) + "%";

            let revenue = document.createElement('td');
            revenue.textContent = product["revenue"].toFixed(2);

            let totalRevenue = document.createElement('td');
            totalRevenue.textContent = product["totalRevenue"].toFixed(2)   ;

            newRow.appendChild(id);
            newRow.appendChild(name);
            newRow.appendChild(sold);
            newRow.appendChild(percent);
            newRow.appendChild(price);
            newRow.appendChild(revenue);
            newRow.appendChild(totalRevenue);

            if (i % 2 === 0) newRow.style.backgroundColor = "rgb(237, 237, 237)";

            if (statsTable) statsTable.appendChild(newRow);

            i++;
        }
    }
}

async function searchProducts() {
    let input = document.getElementById("productsFilterInput").value;
    let res;

    switch (selectedProductFilter) {
        case "All":
            res = await retrieveProducts();
            break;
        case "Id":
            res = await retrieveProductById(input);
            break;

        case "Name":
            res = await retrieveProductByName(input, "name");
            break;
        case "Price":
            retrieveProductsByDate(input, "price");
            break;
        case "Date":
            retrieveProductsByDate(input, "date");
            break;
        case "Week":
            retrieveProductsByDate(input, "week");
            break;
        case "Month":
            retrieveProductsByDate(input, "month");
            break;
        case "Year":
            retrieveProductsByDate(input, "year");
            break;
        default:
            console.log("Error selecting filter");
            break;
    }
}

let initialState = "";
function switchUpOrDown(clickedHeader) {
    const parent = clickedHeader.parentNode;
    const up = clickedHeader.children[1];
    const upStyle = window.getComputedStyle(up);
    const down = clickedHeader.children[2];
    const downStyle = window.getComputedStyle(down);

    for (let ch of parent.children) {
        let upCh = ch.children[1];
        let downCh = ch.children[2];

        if (ch !== clickedHeader) {
            upCh.style.display = "none";
            downCh.style.display = "none";
        }
    }

    if (upStyle.display === "none" && downStyle.display === "none") {
        initialState = document.getElementById("productsTable").innerHTML;
        up.style.display = "flex";

        return ["asc", initialState];
    }
    else if (upStyle.display === "flex" && downStyle.display === "none") {
        up.style.display = "none";

        down.style.display = "flex";

        return ["desc", initialState];
    }
    else {
        up.style.display = "none";
        down.style.display = "none";
    }

    return ["", initialState];
}

function extractDataFromProductsTable() {
    let table = document.getElementById("productsTable"); 
    let rows = table.getElementsByTagName('tr');
    let data = [];
    let tableHeaders = table.querySelectorAll("th");

    for (let i = 1; i < rows.length; i++) {
        let cells = rows[i].getElementsByTagName('td');
        let rowData = {};

        for (let j = 0; j < cells.length; j++) {
            const currentHeader = tableHeaders[j % tableHeaders.length].children[0].textContent;

            rowData[currentHeader] = cells[j].textContent.trim();
            if (rowData[currentHeader][rowData[currentHeader].length - 1] === "%"){
                console.log("fff");
                rowData[currentHeader] = rowData[currentHeader].slice(0, -1);
            }
        }

        data.push(rowData);
    }

    return data; 
}

function sortProductsBy(clickedHeader) {
    let res = switchUpOrDown(clickedHeader);
    let headerNumber = 0;
    const tableHeaders = clickedHeader.parentNode.children;
    const parent = clickedHeader.parentNode;
    const table = document.getElementById("productsTable");

    for (let i in tableHeaders) {
        if (clickedHeader === tableHeaders[i]) {
            headerNumber = i;
            break;
        }
    }

    let data = extractDataFromProductsTable();
    console.log(data);
    const clickedHeaderTextContent = clickedHeader.children[0].textContent;
    console.log("index", clickedHeaderTextContent);
    switch (res[0]) {
        case "asc":
            if (clickedHeaderTextContent !== "Name") {
                data.sort((a, b) => { return (b[clickedHeaderTextContent] - a[clickedHeaderTextContent]) });
            }
            else {
                data.sort((a, b) => a.Name.localeCompare(b.Name));
            }
            console.log("asc");
            table.children[1].innerHTML = "";

            break;
        case "desc":
            if (clickedHeaderTextContent !== "Name") {
                data.sort((a, b) => { return (a[clickedHeaderTextContent] - b[clickedHeaderTextContent]) });
            }
            else {
                data.sort((a, b) => b.Name.localeCompare(a.Name));
            }
            console.log("desc");
            table.children[1].innerHTML = "";

            break;
        default:
            table.innerHTML = res[1];
            console.log("none");

            break;
    }

    if (table.children[1].innerHTML === "") {
        let i = 0;
        for (let row of data) {
            let tr = document.createElement("tr");
            for (let col in row) {
                let td = document.createElement("td");
                td.textContent = row[col];

                if (col === "Percent") td.textContent += "%";

                tr.appendChild(td);
            }

            if (i % 2 === 0) tr.style.backgroundColor = "rgb(237, 237, 237)";
            i++;

            table.children[1].appendChild(tr);
        }
    }

    console.log(data);
}