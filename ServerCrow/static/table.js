// Init
let currentGridNumber = 1;
let currentButton = document.getElementById("productsPagesButton" + 1);
currentButton.style.backgroundColor = "#0a3677";
currentButton.style.color = "white";
currentButton.style.border = "1px solid black";

// First row
const n_table = document.getElementById("numTable");

// Third row
const multiplier_array = [1, 2, 5];
let multiplier_index = 0;

// Fourth row
const li = document.getElementsByClassName("grid-product");
let page_number = 1;

// Ticket
let data = [];
const ticketList = document.getElementById("ticketList");
const ticketProducts = document.getElementsByClassName("ticketProduct");
let current_ticket = [];
let added_ticket = [];
let modified_ticket = [];
let deleted_ticket = [];
const lastProduct = document.getElementById("lastOrder");
const price = document.getElementById("price");
let price_val = parseFloat(price.textContent);

// Hide the current products division if there are no products in the ticket
if (ticketProducts.length > 0) document.getElementById("divisionCurrentProducts").style.display = "flex";

for (let i = 0; i < ticketProducts.length; i++) {
    const t = parseInt(((ticketProducts[i].children)[0].textContent).substr(1));
    const n = (ticketProducts[i].children)[1].textContent;
    const p = parseFloat((ticketProducts[i].children)[2].textContent);

    current_ticket.push({ times: t, name: n, price: p });
}

//if (current_ticket.length > 0) lastProduct.textContent = current_ticket[added_ticket.length - 1];
lastProduct.textContent = "-";

// Ticket
let modifyingProduct;

function goBack() {
    setTimeout(() => { window.location.href = "/"; }, 10);
}
function deleteLastOrder() {
    if (added_ticket.length > 0) {
        let last = added_ticket[added_ticket.length - 1];
        const discount = document.getElementById("discountValue").textContent;
        price.textContent = (parseFloat(price.textContent) - (parseFloat(last.price) * parseInt(last.times) * (1.0 - parseFloat(discount) / 100.0))).toFixed(2); // TODO: Change por removed product price

        // Remove the li item
        let modifyDeleteMenu = document.getElementById("modifyDeleteMenu");
        if (ticketList.lastElementChild.contains(modifyDeleteMenu)) {
            modifyDeleteMenu.style.display = "none";
            document.body.appendChild(modifyDeleteMenu);
        }
        ticketList.removeChild(ticketList.lastElementChild);

        added_ticket.pop();
        last = added_ticket[added_ticket.length - 1];

        // Remove from ticket array
        let found = added_ticket.find(prod => prod.name === last.name);
        if (added_ticket.length > 0) lastProduct.textContent = "x" + found["times"] + " " + last.name + " | " + last.price;
        else lastProduct.textContent = "-";
    }
    else {
        lastProduct.textContent = "-";
    }
}

function toggleMultiplier() {
    let current_multiplier = document.getElementById(multiplier_array[multiplier_index].toString());
    multiplier_index = (multiplier_index + 1) % multiplier_array.length;
    let next_multiplier = document.getElementById((multiplier_array[multiplier_index]).toString());

    current_multiplier.style.display = "none";
    next_multiplier.style.display = "flex";
}

function addProductToTicket(clickedProduct) {
    const secondRow = document.getElementById("secondRow");
    secondRow.classList.toggle('clicked');
    setTimeout(function () {
        secondRow.classList.remove('clicked');
    }, 125);

    const price = document.getElementById("price");
    const discount = document.getElementById("discountValue").textContent;

    let last = "x" +  + added_ticket[added_ticket.length - 1];
    let found = added_ticket.find(prod => prod.name === (clickedProduct.children)[0].textContent);

    if (added_ticket.length === 0) document.getElementById("divisionAddedProducts").style.display = "flex";

    if (found) {
        found["times"] += multiplier_array[multiplier_index];

        const text = found.name;

        let f = Array.from(document.getElementsByClassName("addedTicketProduct")).find(element => element.children[1].textContent.includes(text));

        f.children[0].children[0].textContent = "x" + found.times;
        f.children[3].textContent = (found.times * found.price).toFixed(2);

        lastProduct.textContent = "x" + found["times"] + " " + found.name + " | " + found.price;

        price.textContent = (parseFloat(price.textContent) + (parseFloat(found.price) * multiplier_array[multiplier_index] * (1.0 - parseFloat(discount) / 100.0))).toFixed(2);
    }
    else {
        let product = { times: multiplier_array[multiplier_index], name: (clickedProduct.children)[0].textContent, price: parseFloat((clickedProduct.children)[1].textContent).toFixed(2) };
        added_ticket.push(product);

        last = added_ticket[added_ticket.length - 1];
        lastProduct.textContent = "x" + multiplier_array[multiplier_index] + " " + last.name + " | " + last.price;

        // Add the product in the ticket of the table
        let child = document.createElement("li");
        child.className = "addedTicketProduct";
        child.backgroundColor = "green";
        child.onclick = function () {
            openModifyDeleteMenu(this);
        };

        let child_product_times = document.createElement("div");
        child_product_times.className = "productTimes";
        child_product_times.textContent = "x" + multiplier_array[multiplier_index];

        let child_old_times = document.createElement("div");
        child_old_times.className = "oldProductTimes";

        let child_times = document.createElement("div");
        child_times.className = "times";
        child_times.appendChild(child_product_times);
        child_times.appendChild(child_old_times);
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

        price.textContent = (parseFloat(price.textContent) + (parseFloat(last.price) * multiplier_array[multiplier_index] * (1.0 - parseFloat(discount) / 100.0))).toFixed(2);
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
    const modifyDeleteMenu = document.getElementById("modifyDeleteMenu");
    ProductsTab.style.visibility = 'visible';
    TicketTab.style.visibility = 'hidden';
    modifyDeleteMenu.style.visibility = 'hidden';

    const productsMenu = document.getElementById("productsMenu");
    const ticketMenu = document.getElementById("ticketMenu");
    productsMenu.style.display = 'block';
    ticketMenu.style.display = 'none';
}


function changeToTicketTab() {
    const ProductsTab = document.getElementById("tab-buttonProducts");
    const TicketTab = document.getElementById("tab-buttonTicket");
    const modifyDeleteMenu = document.getElementById("modifyDeleteMenu");
    ProductsTab.style.visibility = 'hidden';
    TicketTab.style.visibility = 'visible';
    modifyDeleteMenu.style.visibility = 'visible';

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

    const data = {
        order: current_ticket,
        added: added_ticket,
        modified: modified_ticket, 
        deleted: deleted_ticket,
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
    const display2 = document.getElementById('amountInput');
    
    if (display.value <= 999) display.value += number;
    if (display2.value <= 999) display2.value += number;
}

function clearDisplay() {
    const display = document.getElementById('tableInput');
    const display2 = document.getElementById('amountInput');

    display.value = "";
    display2.value = "";
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

function openModifyDeleteMenu(clickedProduct) {
    let modifyDeleteMenu = document.getElementById("modifyDeleteMenu");

    clickedProduct.appendChild(document.getElementById("modifyDeleteMenu"));

    if (modifyingProduct == clickedProduct) { // First selected item?
        modifyingProduct = null;

        if (clickedProduct.style.backgroundColor !== "rgb(255, 120, 151)") {
            if (clickedProduct.className == "ticketProduct") {
                clickedProduct.style.backgroundColor = "white";
            }
            else if (clickedProduct.className == "addedTicketProduct") {
                clickedProduct.style.backgroundColor = "#D7FCDA";
            }
            else {
                clickedProduct.style.backgroundColor = "orange";
            }
        }
        else {
            clickedProduct.style.backgroundColor = "rgb(255, 120, 151)";
        }

        modifyDeleteMenu.style.display = "none";
    }
    else {
        if (clickedProduct.style.backgroundColor !== "rgb(255, 120, 151)") { // Not deleted item?
            if (clickedProduct.className == "ticketProduct") {
                clickedProduct.style.backgroundColor = "yellow"
            }
            else if (clickedProduct.className == "addedTicketProduct") {
                clickedProduct.style.backgroundColor = "#e9fe6d";
            }
            else {
                clickedProduct.style.backgroundColor = "orange";
            }
        }
        else {

        }

        if (modifyingProduct == null) {
            modifyDeleteMenu.style.display = "flex";
        }
        else {
            if (modifyingProduct.style.backgroundColor !== "rgb(255, 120, 151)") {
                if (modifyingProduct.className == "ticketProduct") {
                    modifyingProduct.style.backgroundColor = "white";
                }
                else if (modifyingProduct.className == "addedTicketProduct") {
                    modifyingProduct.style.backgroundColor = "#D7FCDA";
                }
                else {
                    modifyingProduct.style.backgroundColor = "orange";
                }
            }
            else {
                modifyingProduct.style.backgroundColor = "rgb(255, 120, 151)";
            }
            modifyDeleteMenu.style.display = "flex";
        }

        modifyingProduct = clickedProduct;
    }
}

function selectProductToModify() {
    event.stopPropagation(); // So the child onclick is on top of the parents'

    const modifyProductMenu = document.getElementById("modifyProductMenu");

    modifyProductMenu.style.display = "flex";
    modifyProductMenu.style.zIndex = "6";

    let tab = document.getElementsByClassName("tab");
    let ticketMenu = document.getElementById("ticketMenu");

    tab[0].style.pointerEvents = "none";
    tab[0].style.filter = "blur(5px)";
    ticketMenu.style.pointerEvents = "none";
    ticketMenu.style.filter = "blur(5px)";
}
function modifyProduct() {
    let amountInput = document.getElementById("amountInput");
    let productTimes = modifyingProduct.children[0].children[0];
    let oldProductTimes = modifyingProduct.children[0].children[1];

    if (modifyingProduct.children[0].children[1].textContent === "") {
        oldProductTimes.textContent = productTimes.textContent;
        oldProductTimes.style.textDecoration = "line-through";

        productTimes.style.textDecoration = "none";
        productTimes.textContent = "x" + amountInput.value;
    }
    else {
        productTimes.textContent = "x" + amountInput.value;
    }

    modifyingProduct.style.backgroundColor = "orange";
    modifyingProduct.className = "modifiedProduct";
    amountInput.value = "";

    const discount = document.getElementById("discountValue").textContent;
    price.textContent = (parseFloat(price.textContent) - (parseFloat(modifyingProduct.children[3].textContent) * (1.0 - parseFloat(discount) / 100.0))).toFixed(2);

    modifyingProduct.children[3].textContent = (parseFloat(modifyingProduct.children[2].textContent) * parseFloat(productTimes.textContent.substr(1))).toFixed(2);  
    price.textContent = (parseFloat(price.textContent) + (parseFloat(modifyingProduct.children[3].textContent) * (1.0 - parseFloat(discount) / 100.0))).toFixed(2);

    let index = added_ticket.findIndex(prod => prod.name === modifyingProduct.children[1].textContent);
    if (index !== -1) {
        added_ticket[index].times = modifyingProduct.children[0].children[0].textContent.substr(1);
    }

    let modified_prod = {
        new_amount: modifyingProduct.children[0].children[0].textContent.substr(1),
        name: modifyingProduct.children[1].textContent,
        price: modifyingProduct.children[2].textContent
    };

    modified_ticket.push(modified_prod);

    cancelModifyDeleteMenu();
}

function cancelModifyDeleteMenu() {
    const modifyProductMenu = document.getElementById("modifyProductMenu");
    const amountInput = document.getElementById("amountInput");

    modifyProductMenu.style.display = "none";

    let tab = document.getElementsByClassName("tab");
    let ticketMenu = document.getElementById("ticketMenu");

    tab[0].style.pointerEvents = "auto"
    tab[0].style.filter = "blur(0px)";
    ticketMenu.style.pointerEvents = "auto";
    ticketMenu.style.filter = "blur(0px)";

    amountInput.value = "";
}

function selectProductToDelete(clickedElement) {
    event.stopPropagation(); // So the child onclick is on top of the parents'

    let a = document.getElementById("modifyDeleteMenu").parentNode;
    a.style.backgroundColor === "rgb(255, 120, 151)" ? a.className === "ticketProduct" ? a.style.backgroundColor = "white" : a.style.backgroundColor = "#e9fe6d" : a.style.backgroundColor = "#ff7897";

    if (a.className === "ticketProduct") {
        let index = deleted_ticket.findIndex(prod => prod.name === a.children[1].textContent);
        if (index === -1) {
            let prod = {
                times: a.children[0].textContent.substr(1),
                name: a.children[1].textContent,
                price: a.children[2].textContent
            }

            deleted_ticket.push(prod);
        }
        else {
            deleted_ticket.splice(index, 1);
        }
    }
    else {
        let index = added_ticket.findIndex(prod => prod.name === a.children[1].textContent);
        if (index !== -1) {
            let last = added_ticket[added_ticket.length - 1];
            const price = document.getElementById("price");
            const discount = document.getElementById("discount").textContent;

            price.textContent = (parseFloat(price.textContent) - (parseFloat(added_ticket[index].price) * added_ticket[index].times * (1.0 - parseFloat(discount) / 100.0))).toFixed(2);
            console.log("b", added_ticket[index].times);
            added_ticket.splice(index, 1);

            if (added_ticket.length > 0) {
                last = added_ticket[added_ticket.length - 1];
                lastProduct.textContent = "x" + last.times + " " + last.name + " | " + last.price;
            }
            else lastProduct.textContent = "-";

            let modifyDeleteMenu = document.getElementById("modifyDeleteMenu");
            modifyDeleteMenu.style.display = "none";
            document.body.appendChild(modifyDeleteMenu);

            a.remove();
        }
    }
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


