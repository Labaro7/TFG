// Init
let currentGridNumber = 1;
let currentButton = document.getElementById("productsPagesButton" + 1);
currentButton.style.backgroundColor = "#0a3677";
currentButton.style.color = "white";
currentButton.style.border = "1px solid black";

// First row
const n_table = document.getElementById("numTable");

// Fourth row
const li = document.getElementsByClassName("grid-product");
let page_number = 1;

// Ticket
let data = [];
const ticketList = document.getElementById("ticketList");
const ticketProducts = document.getElementsByClassName("ticketProduct");
let current_ticket = [];
let added_ticket = [];
const lastProduct = document.getElementById("lastOrder");
const price = document.getElementById("price");
let price_val = parseFloat(price.textContent);

for (let i = 0; i < ticketProducts.length; i++) {
    const t = parseInt(((ticketProducts[i].children)[0].textContent).substr(1));
    const n = (ticketProducts[i].children)[1].textContent;
    const p = parseFloat((ticketProducts[i].children)[2].textContent);

    current_ticket.push({ times: t, name: n, price: p });
}

//if (current_ticket.length > 0) lastProduct.textContent = current_ticket[added_ticket.length - 1];
lastProduct.textContent = "-";

function goBack() {
    setTimeout(() => { window.location.href = "/"; }, 10);
}
function deleteLastOrder() {
    if (added_ticket.length > 0) {
        let last = added_ticket[added_ticket.length - 1];
        const discount = document.getElementById("discountValue").textContent;
        price.textContent = (parseFloat(price.textContent) - (parseFloat(last.price) * parseInt(last.times) * (1.0 - parseFloat(discount) / 100.0))).toFixed(2); // TODO: Change por removed product price

        // Remove the li item
        ticketList.removeChild(ticketList.lastElementChild);

        added_ticket.pop();
        last = added_ticket[added_ticket.length - 1];

        // Remove from ticket array
        if (added_ticket.length > 0) lastProduct.textContent = last.name + " | " + last.price;
        else lastProduct.textContent = "-";
    }
    else {
        lastProduct.textContent = "-";
    }

}

function addProductToTicket(clickedProduct) {
    const secondRow = document.getElementById("secondRow");
    secondRow.classList.toggle('clicked');
    setTimeout(function () {
        secondRow.classList.remove('clicked');
    }, 125); // Glow animation

    // Add the product to the list of products of the table
    const price = document.getElementById("price");
    const discount = document.getElementById("discountValue").textContent;

    let last = added_ticket[added_ticket.length - 1];
    let found = added_ticket.find(prod => prod.name === (clickedProduct.children)[0].textContent);

    if (found) {
        found["times"]++;

        const text = found.name;

        let f = Array.from(document.getElementsByClassName("addedTicketProduct")).find(element => element.children[1].textContent.includes(text));

        f.children[0].textContent = "x" + found.times;
        f.children[3].textContent = (found.times * found.price).toFixed(2);

        lastProduct.textContent = found.name + " | " + found.price;

        price.textContent = (parseFloat(price.textContent) + (parseFloat(found.price) * (1.0 - parseFloat(discount) / 100.0))).toFixed(2);
    }
    else {
        let product = { times: 1, name: (clickedProduct.children)[0].textContent, price: parseFloat((clickedProduct.children)[1].textContent).toFixed(2) };
        added_ticket.push(product);

        last = added_ticket[added_ticket.length - 1];
        lastProduct.textContent = last.name + " | " + last.price;

        // Add the product in the ticket of the table
        let child = document.createElement("li");
        child.className = "addedTicketProduct";
        child.backgroundColor = "green";

        let child_times = document.createElement("div");
        child_times.className = "productTimes";
        child_times.textContent = "x" + 1;
        child.appendChild(child_times);

        let child_name = document.createElement("div");
        child_name.className = "productNames";
        child_name.textContent = product["name"];
        child.appendChild(child_name);

        let child_price = document.createElement("div");
        child_price.className = "productPrices";
        child_price.textContent = product["price"];
        child.appendChild(child_price);

        let child_total_price = document.createElement("div");
        child_total_price.className = "productTotalPrices";
        child_total_price.textContent = (product["times"] * product["price"]).toFixed(2);
        child.appendChild(child_total_price);
        
        ticketList.appendChild(child);

        price.textContent = (parseFloat(price.textContent) + (parseFloat(last.price) * (1.0 - parseFloat(discount) / 100.0))).toFixed(2);
    }
}

function openDeployable(clickedDeployable) {
    const grid = "grid" + page_number;
    const grid_products = document.getElementById(grid).querySelectorAll("li");
    for (let i of grid_products) {
        i.style.display = "none";
    }

    const clickedDeployableProducts = document.querySelectorAll("li[data-deployable='" + clickedDeployable.dataset.id + "']");
    for (let i of clickedDeployableProducts) {
        i.style.display = "flex";
    }
}


function changeToProductsTab() {
    const ProductsTab = document.getElementById("tab-buttonProducts");
    const TicketTab = document.getElementById("tab-buttonTicket");
    ProductsTab.style.visibility = 'visible';
    TicketTab.style.visibility = 'hidden';

    const productsMenu = document.getElementById("productsMenu");
    const ticketMenu = document.getElementById("ticketMenu");
    productsMenu.style.display = 'block';
    ticketMenu.style.display = 'none';
}


function changeToTicketTab() {
    const ProductsTab = document.getElementById("tab-buttonProducts");
    const TicketTab = document.getElementById("tab-buttonTicket");
    ProductsTab.style.visibility = 'hidden';
    TicketTab.style.visibility = 'visible';

    const productsMenu = document.getElementById("productsMenu");
    const ticketMenu = document.getElementById("ticketMenu");
    productsMenu.style.display = 'none';
    ticketMenu.style.display = 'flex';
}


function showPage(pageId) {
    const pages = document.querySelectorAll('.page');
    pages.forEach(page => {
        page.style.display = 'none';
    });

    const selectedPage = document.getElementById(`${pageId}-page`);
    if (selectedPage) {
        selectedPage.style.display = 'block';
    }
}


function selectButton(number) {
    let button = document.getElementById("productsPagesButton" + number);
    currentButton.style.backgroundColor = "white";
    currentButton.style.color = "black";
    currentButton.style.border = "1px solid #0a3677";
    currentButton = button;

    button = document.getElementById("productsPagesButton" + number);
    button.style.backgroundColor = "#0a3677";
    button.style.color = "white";
    currentButton.style.border = "1px solid black";
}


// TODO: It cannot display 6 or more menus
function displayMenu(clickedButton) {
    const number = clickedButton.getAttribute("data-number");
    page_number = number;
    currentGrid = document.getElementById("grid"+currentGridNumber);
    
    if (currentGrid) {
        currentGrid.style.display = 'none';
        currentGridNumber = number;

        currentGrid = document.getElementById("grid" + currentGridNumber);
        button = document.getElementById("productsPagesButton" + currentGridNumber);
        currentGrid.style.display = 'flex';
        const currentGridDeployableElements = currentGrid.querySelectorAll("li");

        for (let i of currentGridDeployableElements) {
            i.style.display = "none";
        }

        const currentGridElements = currentGrid.children;
        for (let i of currentGridElements) {
            i.style.display = "flex";
        }

        const currentGridDeployables = currentGrid.querySelectorAll(".deployable-product");
        for (let i of currentGridDeployables) {
            i.style.display = "none";
        }
        selectButton(currentGridNumber);
    }
}


function saveOrder() {
    const n_table = parseInt((document.getElementById("numTable").textContent).substr(7)); // Use substr(7) to eliminate "Table: "
    const order = current_ticket;
    const added = added_ticket;
    const data = {
        order: order,
        added: added,
        n_table: n_table,
        employee: getCookie("employee_name")
    };

    const url = "/order";

    fetch(url, {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify(data),
    })
        .then(response => response.text())
        .then(data => {
            //console.log('Response:', data);
        })
        .catch(error => {
            //console.error('Error:', error);
        });

    // If setTimeout is not used, when the index.html is received
    // it doesn't have the tables prices updated because
    // it didn't have enough time to retrieve the new information
    setTimeout(() => { window.location.href = "/"; }, 100);
}

function openMoveTableMenu() {
    let tab = document.getElementsByClassName("tab");
    let productsMenu = document.getElementById("productsMenu");
    let moveTableMenu = document.getElementById("moveTableMenu");

    tab[0].style.pointerEvents = "none";
    tab[0].style.filter = "blur(5px)";
    productsMenu.style.pointerEvents = "none";
    productsMenu.style.filter = "blur(5px)";
    moveTableMenu.style.display = "flex"; 
}

function appendNumber(number) {
    const display = document.getElementById('tableInput');
    display.value += number;
}

function clearDisplay() {
    const display = document.getElementById('tableInput');
    display.value = '';
}

function acceptMoveTableMenu() {
    const api_moveTable = "/moveTable";

    let _current_n_table = parseInt(n_table.textContent.substr(7));
    let _new_n_table = parseInt(document.getElementById("tableInput").value);
    let data = {
        current_n_table: _current_n_table,
        new_n_table: _new_n_table,
        employee: getCookie("employee_name")
    };

    fetch(api_moveTable, {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify(data),
    })
        .then(response => response.text())
        .then(data => {
            //console.log('Response:', data);
        })
        .catch(error => {
            //console.error('Error:', error);
        });

    setTimeout(() => { window.location.href = "/"; }, 100);
}

function cancelMoveTableMenu() {
    let tab = document.getElementsByClassName("tab");
    let productsMenu = document.getElementById("productsMenu");
    let moveTableMenu = document.getElementById("moveTableMenu");
    let moveTableWarningMenu = document.getElementById("moveTableWarningMenu");

    tab[0].style.pointerEvents = "auto";
    tab[0].style.filter = "blur(0px)";
    productsMenu.style.pointerEvents = "auto";
    productsMenu.style.filter = "blur(0px)";
    moveTableMenu.style.display = "none";
    moveTableWarningMenu.style.display = "none";

}

function moveTable() {
    const current_n_table = parseInt(n_table.textContent.substr(7));
    const new_n_table = parseInt(document.getElementById("tableInput").value);

    const api_currentTables = "/currentTables";
    const api_moveTable = "/moveTable";

    fetch(api_currentTables)
        .then(response => response.json())
        .then(data => {
            //console.log('Response:', data);
            for (let i of data["tables"]) {
                if (i === parseInt(new_n_table)) {
                    // Display moveTableWarning
                    const moveTableMenu = document.getElementById("moveTableMenu");
                    moveTableMenu.style.display = "none";

                    let tab = document.getElementsByClassName("tab");
                    let productsMenu = document.getElementById("productsMenu");
                    const warning = document.getElementById("moveTableWarningMenu");

                    tab[0].style.pointerEvents = "none";
                    tab[0].style.filter = "blur(5px)";
                    productsMenu.style.pointerEvents = "none";
                    productsMenu.style.filter = "blur(5px)";
                    warning.style.display = "flex";

                    return;
                }
            }

            data = {
                current_n_table: current_n_table,
                new_n_table: new_n_table,
                employee: getCookie("employee_name")
            };

            fetch(api_moveTable, {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json',
                    'Set-Cookie': getCookie("session_token"),
                    'Set-Cookie': getCookie("employee_name")
                },
                body: JSON.stringify(data),
            })
                .then(response => response.text())
                .then(data => {
                    console.log('Responsaal:', data);
                })
                .catch(error => {
                    console.error('Errool:', error);
                });

            setTimeout(() => { window.location.href = "/"; }, 100);
        })
        .catch(error => {
            console.error('Error:', error);
        });
}

function payTable() {
    let data = {
        n_table: n_table.textContent.substr(7), //substr to delete "Table: "
        ticket : current_ticket,
        price: price.textContent,
        date: new Date(),
        //method: cash or card
    }
    if (getCookie("employee_name") != null) {
        data.employee = getCookie("employee_name")
    }

    console.log(data);

    const url = "/payTable";
    fetch(url, {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify(data),
    })
        .then(response => response.text())
        .then(data => {
            //console.log('Response:', data);
        })
        .catch(error => {
            //console.error('Error:', error);
        });

    setTimeout(() => { window.location.href = "/"; }, 100);
}

function openDeleteTableWarningMenu() {
    const deleteTableWarningMenu = document.getElementById("deleteTableWarningMenu");
    let tab = document.getElementsByClassName("tab");
    let ticketMenu = document.getElementById("ticketMenu");

    deleteTableWarningMenu.style.display = "flex";
    tab[0].style.pointerEvents = "none";
    tab[0].style.filter = "blur(5px)";
    ticketMenu.style.pointerEvents = "none";
    ticketMenu.style.filter = "blur(5px)";
}

function cancelDeleteTableWarningMenu() {
    const deleteTableWarningMenu = document.getElementById("deleteTableWarningMenu");
    let tab = document.getElementsByClassName("tab");
    let ticketMenu = document.getElementById("ticketMenu");

    deleteTableWarningMenu.style.display = "none";
    tab[0].style.pointerEvents = "auto";
    tab[0].style.filter = "blur(0px)";
    ticketMenu.style.pointerEvents = "auto";
    ticketMenu.style.filter = "blur(0px)";
}

function deleteTable() {
    let data = {
        n_table: n_table.textContent.substr(7),
        employee: "Adri"
    }

    const url = "/deleteTable";
    fetch(url, {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify(data),
    })
        .then(response => response.text())
        .then(data => {
            //console.log('Response:', data);
        })
        .catch(error => {
            //console.error('Error:', error);
        });

    setTimeout(() => { window.location.href = "/"; }, 100);
}

function openPayTableWarningMenu() {
    const payTableWarningMenu = document.getElementById("payTableWarningMenu");
    let tab = document.getElementsByClassName("tab");
    let ticketMenu = document.getElementById("ticketMenu");

    payTableWarningMenu.style.display = "flex";
    tab[0].style.pointerEvents = "none";
    tab[0].style.filter = "blur(5px)";
    ticketMenu.style.pointerEvents = "none";
    ticketMenu.style.filter = "blur(5px)";
}

function cancelPayTableWarningMenu() {
    const payTableWarningMenu = document.getElementById("payTableWarningMenu");
    let tab = document.getElementsByClassName("tab");
    let ticketMenu = document.getElementById("ticketMenu");

    payTableWarningMenu.style.display = "none";
    tab[0].style.pointerEvents = "auto";
    tab[0].style.filter = "blur(0px)";
    ticketMenu.style.pointerEvents = "auto";
    ticketMenu.style.filter = "blur(0px)";
}

function openModifyProductMenu(clickedProduct) {
    console.log(clickedProduct.children[0].textContent);
}

function modifyTable() {

}


function getCookie(cookieName) {
    const name = cookieName + "=";
    const decodedCookie = decodeURIComponent(document.cookie);
    const cookieArray = decodedCookie.split(';');

    for (let i = 0; i < cookieArray.length; i++) {
        let cookie = cookieArray[i];
        while (cookie.charAt(0) === ' ') {
            cookie = cookie.substring(1);
        }
        if (cookie.indexOf(name) === 0) {
            return cookie.substring(name.length, cookie.length);
        }
    }

    return null;
}


