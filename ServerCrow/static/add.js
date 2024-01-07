// Init
let tabNumber = 1;
let currentGridNumber = 1;
let currentButton = document.getElementById("productsPagesButton" + 1);
currentButton.style.backgroundColor = "#0a3677";
currentButton.style.color = "white";
currentButton.style.border = "1px solid black";
let selectedDeployable = 0;
let selectedPage = 1;

// Fourth row
let page_number = 1;

function changeTab(clickedTab) {
    const currentPage = document.querySelector('div[data-page="' + tabNumber + '"]');
    let currentTab = document.querySelector("div[data-tab='" + tabNumber + "']");
    currentPage.style.display = "none";
    currentTab.children[1].style.visibility = "hidden";

    tabNumber = clickedTab.getAttribute("data-tab");

    const newPage = document.querySelector('div[data-page="' + tabNumber + '"]');
    currentTab = document.querySelector("div[data-tab='" + tabNumber + "']");
    newPage.style.display = "flex";
    currentTab.children[1].style.visibility = "visible"; 
}

// TODO: It cannot display 6 or more menus
function displayMenu(clickedButton) {
    selectedDeployable = 0;
    selectedPage = clickedButton.textContent;
    const number = clickedButton.getAttribute("data-number");
    page_number = number;
    currentGrid = document.getElementById("grid" + currentGridNumber);

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

function openDeployable(clickedDeployable) {
    selectedDeployable = clickedDeployable.dataset.id;
    const grid = "grid" + page_number;
    const grid_products = document.getElementById(grid).querySelectorAll("li");
    for (let i of grid_products) {
        i.style.display = "none";
    }

    const clickedDeployableProducts = document.querySelectorAll("li[data-name='" + clickedDeployable.textContent);
    for (let i of clickedDeployableProducts) {
        i.style.display = "flex";
    }
}

function addProduct() {
    const name = document.getElementById("nameInput").value;
    let price = document.getElementById("priceInput").value;
    const color = document.getElementById("colorInput").value;
    //const page = document.getElementById("pageInput").value;
    let deployable = document.getElementById("deployableInput").value;

    deployable == "Deployable" ? (deployable = 0, price = "0") : deployable = selectedDeployable;

    const data = {
        name: name,
        price: price,
        color: color,
        page: selectedPage,
        deployable: deployable,
    };

    const url = 'https://192.168.1.66:18080/add/product';
    fetch(url, {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify(data),
    })
        .then(response => response.text())
        .then(data => {
            console.log('Response:', data);
        })
        .catch(error => {
            console.error('Error:', error);
        });

    setTimeout(() => { window.location.href = "https://192.168.1.66:18080/add"; }, 100);
}