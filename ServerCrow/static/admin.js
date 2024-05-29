/*import * as Stats from "./admin_stats.js";
import * as Products from "./admin_products.js";
import * as Employees from "./admin_employees.js";
import * as ProductsAndAllergens from "./admin_productsAndAllergens.js";
import * as Config from "./admin_config.js";*/

const SELECTED_BACKGROUND_COLOR = "rgb(28, 89, 176)";
const SELECTED_TEXT_COLOR = "white";
const UNSELECTED_BACKGROUND_COLOR = "white"
const UNSELECTED_TEXT_COLOR = "black";

const SELECTED_BUTTON_BACKGROUND_COLOR = "rgb(28, 89, 176)";
const UNSELECTED_BUTTON_BACKGROUND_COLOR = "rgb(9, 43, 92)";
const BUTTON_TEXT_COLOR = "white";

// Init
let tabNumber = 1;
let currentGridNumber = 1;
let currentButton = document.getElementById("productsPagesButton" + 1);
let selectedDeployable = 0;
let selectedPage = 1;

// Modify
let selectedElement = document.getElementById("selected-product");
let selectedElementName;
let selectedElementPrice;
let selectedElementPage = selectedPage;

// Fourth row
let page_number = 1;

// Employee
let selectedEmployee = {
    firstName: "",
    lastName: "",
    email: "",
    id: "",
    mobileNumber: "",
    level: "",
    username: "",
    password_hash: "",
};
let selectedEmployeeElement;
let oldEmployee_firstName;
let oldEmployee_lastName;
let oldEmployee_email;
let oldEmployee_id;
let oldEmployee_mobile;
let oldEmployee_level;
let oldEmployee_username;
let oldEmployee_password;

selectAllIngredients();

function changeTab(clickedTab) {
    const currentTab = document.querySelector('div[data-tab="' + tabNumber + '"]');
    const currentSelectedTabIcon = currentTab.querySelector(".selectedTabIcon");
    const currentUnselectedTabIcon = currentTab.querySelector(".unselectedTabIcon");
    const currentPage = document.querySelector('div[data-page="' + tabNumber + '"]');
    currentTab.style.backgroundColor = "rgb(9, 43, 92)";
    currentTab.style.color = "white";
    currentTab.style.borderTop = "5px solid rgb(9, 43, 92)";
    currentTab.style.borderBottom = "1px solid white";
    currentSelectedTabIcon.style.display = "none";
    currentUnselectedTabIcon.style.display = "flex";
    currentPage.style.display = "none";

    tabNumber = clickedTab.getAttribute("data-tab");

    const newTab = document.querySelector('div[data-tab="' + tabNumber + '"]');
    const newSelectedTabIcon = newTab.querySelector(".selectedTabIcon");
    const newUnselectedTabIcon = newTab.querySelector(".unselectedTabIcon");
    const newPage = document.querySelector('div[data-page="' + tabNumber + '"]');
    newTab.style.backgroundColor = "#bdd7fc";
    newTab.style.color = "black";
    newTab.style.borderTop = "5px solid rgb(28, 89, 176)";  
    newTab.style.borderBottom = "0px";
    newSelectedTabIcon.style.display = "flex";
    newUnselectedTabIcon.style.display = "none";

    newPage.style.display = "flex";
}

// TODO: It cannot display 6 or more menus
function displayMenu(clickedButton) {
    selectedDeployable = 0;
    selectedPage = clickedButton.textContent;
    selectedElementPage = selectedPage;
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
    currentButton.style.backgroundColor = "rgb(9, 43, 92)";
    currentButton.style.borderBottom = "1px solid white";
    currentButton = button;

    button = document.getElementById("productsPagesButton" + number);
    button.style.backgroundColor = "rgb(28, 89, 176)";
    currentButton.style.borderBottom = "0px";
}

function openDeployable(clickedDeployable) {
    selectElement(clickedDeployable);

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
    let deployable = document.getElementById("deployableInput").value;

    if (((name && price != 0) && (name && price != 0.0)) || (name && deployable == "Deployable")) {
        deployable == "Deployable" ? (deployable = 0, price = "0") : deployable = selectedDeployable;

        const data = {
            name: name,
            price: price,
            color: color,
            page: selectedPage,
            deployable: deployable,
        };

        const url = "/admin/add/product";
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

        setTimeout(() => { window.location.href = "/admin"; }, 100);
    }
    else {
        alert("Complete the necessary fields");
    }
}

function selectElement(clickedElement) {
    const clonedElement = clickedElement.cloneNode(true);
    clonedElement.id = "selected-product";
    clonedElement.className = "";

    selectedElement = document.getElementById("selectedElement");

    selectedElement.innerHTML = "";
    selectedElement.appendChild(clonedElement);

    if (selectedElement.children[0].hasAttribute("data-id")) {
        selectedElementName = selectedElement.textContent;
        selectedElementPrice = "0";
        newPrice = "0";
    }
    else {
        selectedElementName = selectedElement.children[0].children[0].textContent;
        selectedElementPrice = selectedElement.children[0].children[1].textContent;
    }
}

function modifyProduct() {
    let newName = document.getElementById("newNameInput").value;
    let newPrice = document.getElementById("newPriceInput").value;
    const newColor = document.getElementById("newColorInput").value;

    if (newName !== "") {
        const data = {
            selectedElementName: selectedElementName,
            selectedElementPrice: selectedElementPrice,
            selectedElementPage: selectedElementPage,
            newName: newName,
            newPrice: newPrice,
            newColor: newColor
        };

        // TODO: Load page when response is received
        const url = "/admin/modify/product";
        fetch(url, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify(data),
        })
            .then(response => {
                if (response.ok) setTimeout(() => { window.location.href = "/admin"; }, 100);
            })
            .then(data => {
                console.log('Response:', data);
            })
            .catch(error => {
                console.error('Error:', error);
            });
    }
    else {
        alert("Complete the necessary fields");
    }
}

function deleteProduct() {
    let data = {
        selectedElementName: selectedElementName,
        selectedElementPrice: selectedElementPrice,
        selectedElementPage: selectedElementPage
    }

    const url = "/admin/delete/product";
    fetch(url, {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify(data),
    })
        .then(response => {
            if (response.ok) setTimeout(() => { window.location.href = "/admin"; }, 100);
        })
        .then(data => {
            console.log('Response:', data);
        })
        .catch(error => {
            console.error('Error:', error);
        });
}


/* EMPLOYEE */
function selectEmployee(clickedEmployee) {
    let detailsInputArray = [
        document.getElementById("firstName"),
        document.getElementById("lastName"),
        document.getElementById("email"),
        document.getElementById("id"),
        document.getElementById("mobileNumber"),
        document.getElementById("level"),
        document.getElementById("username"),
        document.getElementById("password_hash")
    ];

    if (selectedEmployeeElement) {
        selectedEmployeeElement.style.backgroundColor = UNSELECTED_BACKGROUND_COLOR;
        selectedEmployeeElement.style.color = UNSELECTED_TEXT_COLOR;
    }

    if (selectedEmployeeElement === clickedEmployee) {
        selectedEmployeeElement.style.backgroundColor = UNSELECTED_BACKGROUND_COLOR;
        selectedEmployeeElement.style.color = UNSELECTED_TEXT_COLOR;
        selectedEmployeeElement = null;

        for (let input of detailsInputArray) input.value = "";
        for (let prop in selectedEmployee) selectedEmployee[prop] = "";
    }
    else {
        selectedEmployeeElement = clickedEmployee;
        selectedEmployeeElement.style.backgroundColor = SELECTED_BACKGROUND_COLOR;
        selectedEmployeeElement.style.color = SELECTED_TEXT_COLOR;

        let i = 0;
        if (clickedEmployee.children[0].children[1]) {
            for (let input of detailsInputArray) {
                input.value = clickedEmployee.children[0].children[1].children[i].textContent;
                i++;
            }

            i = 0;
            for (let prop in selectedEmployee) {
                selectedEmployee[prop] = clickedEmployee.children[0].children[1].children[i].textContent;
                i++;
            }
        }
        else {
            for (let input of detailsInputArray) input.value = "";
        }
    }

    const selectedEmployeeDetails = document.getElementById("selectedEmployeeDetails").children;
    let i = 0;
    for (let div of selectedEmployeeDetails) {
        div.children[0].value = selectedEmployee[div.children[0].id];
        i++;
    }
}
function addEmployee(addEmployeeButton) {
    let employeeList = document.getElementById("employeeList");
    let newEmployee = document.createElement("li");
    let ul = document.createElement("ul");
    let employeeName = document.createElement("li");
    let employeeDetails = document.createElement("ul");

    newEmployee.appendChild(ul);

    employeeName.className = "employeeName";
    newEmployee.appendChild(employeeName);

    employeeDetails.className = "employeeDetails";
    newEmployee.appendChild(employeeDetails);

    newEmployee.className = "employee";
    newEmployee.onclick = function () { selectEmployee(newEmployee); };

    employeeList.appendChild(newEmployee);

    addEmployeeButton.style.display = "none";
}

function addEmployeeDetails() {
    let oldEmployee = {
        firstName: selectedEmployee.firstName ? selectedEmployee.firstName : "",
        lastName: selectedEmployee.lastName ? selectedEmployee.lastName : "",
        email: selectedEmployee.email ? selectedEmployee.email : "",
        id: selectedEmployee.id ? selectedEmployee.id : "",
        mobileNumber: selectedEmployee.mobileNumber ? selectedEmployee.mobileNumber : "",
        level: selectedEmployee.level ? selectedEmployee.level : 0,
        username: selectedEmployee.username ? selectedEmployee.username : "",
        password: selectedEmployee.password_hash ? selectedEmployee.password_hash : "",
    }

    let newEmployee = {
        firstName: document.getElementById("firstName").value,
        lastName: document.getElementById("lastName").value,
        email: document.getElementById("email").value,
        id: document.getElementById("id").value,
        mobileNumber: document.getElementById("mobileNumber").value,
        level: parseInt(document.getElementById("level").value),
        username: document.getElementById("username").value,
        password: document.getElementById("password_hash").value,
    }

    let data = {
        oldEmployee: oldEmployee,
        newEmployee: newEmployee
    }

    const url = "/admin/add/employee";
    fetch(url, {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify(data),
    })
        .then(response => {
            setTimeout(() => { window.location.href = "/admin"; }, 100);
        })
        .then(data => {
            console.log('Response:', data);
        })
        .catch(error => {
            console.error('Error:', error);
        });
}

function deleteEmployeeDetails() {
    let oldEmployee = {
        firstName: selectedEmployee.firstName ? selectedEmployee.firstName : "",
        lastName: selectedEmployee.lastName ? selectedEmployee.lastName : "",
        email: selectedEmployee.email ? selectedEmployee.email : "",
        id: selectedEmployee.id ? selectedEmployee.id : "",
        mobileNumber: selectedEmployee.mobileNumber ? selectedEmployee.mobileNumber : "",
        level: selectedEmployee.level ? selectedEmployee.level : 0,
        username: selectedEmployee.username ? selectedEmployee.username : "",
        password: selectedEmployee.password_hash ? selectedEmployee.password_hash : "",
    }

    let data = {
        oldEmployee: oldEmployee
    }

    const url = "/admin/delete/employee";
    fetch(url, {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify(data),
    })
        .then(response => {
            setTimeout(() => { window.location.href = "/admin"; }, 100);
        })
        .then(data => {
            console.log('Response:', data);
        })
        .catch(error => {
            console.error('Error:', error);
        });
}


/* INGREDIENTS + ALLERGENS */
let selectedProductElement;
let selectedIngredientFilter = document.getElementById("allIngredientsButton");
let added_ingredients = [];
let added_allergens = [];
let selectedProduct;
let selectedIngredients = [];
let selectedAllergens = [];
function selectProduct(clickedProduct) {
    resetDisplay();
    selectedIngredients = [];
    selectedAllergens = [];

    if (selectedProductElement === clickedProduct) {
        selectedProductElement.style.backgroundColor = UNSELECTED_BACKGROUND_COLOR;
        selectedProductElement.style.color = UNSELECTED_TEXT_COLOR;
        selectedProductElement = null;

        selectedProduct = null;

        resetDisplay();
    }
    else {
        if (selectedProductElement) {
            selectedProductElement.style.backgroundColor = UNSELECTED_BACKGROUND_COLOR;
            selectedProductElement.style.color = UNSELECTED_TEXT_COLOR;
        }

        selectedProductElement = clickedProduct;
        selectedProductElement.style.backgroundColor = SELECTED_BACKGROUND_COLOR;
        selectedProductElement.style.color = SELECTED_TEXT_COLOR;

        selectedProduct = clickedProduct.children[0].textContent;

        displayIngredients(clickedProduct);
        displayAllergens(clickedProduct);
    }
}

function resetDisplay() {
    let currentIngredients = document.getElementById("currentIngredientsList");
    let ownedIngredients = document.getElementById("ownedIngredientsList");
    let notOwnedIngredients = document.getElementById("notOwnedIngredientsList");
    let currentAllergens = document.getElementById("currentAllergensList").children;

    for (let ingredient of currentIngredients.children) {
        ingredient.style.background = UNSELECTED_BACKGROUND_COLOR;
        ingredient.style.color = UNSELECTED_TEXT_COLOR;
        ingredient.style.display = "flex";
    }

    ownedIngredients.innerHTML = "";
    notOwnedIngredients.innerHTML = currentIngredients.innerHTML;

    for (let allergen of currentAllergens) {
        allergen.style.background = UNSELECTED_BACKGROUND_COLOR;
        allergen.style.color = UNSELECTED_TEXT_COLOR;
        allergen.style.display = "flex";
    }
}

function removeElementByName(arr, name) {
    const index = arr.findIndex(item => item === name);

    if (index !== -1) {
        arr.splice(index, 1);
    }
}

function selectIngredient(clickedIngredient) {
    if (clickedIngredient.style.backgroundColor === SELECTED_BACKGROUND_COLOR) {
        clickedIngredient.style.background = UNSELECTED_BACKGROUND_COLOR;
        clickedIngredient.style.color = UNSELECTED_TEXT_COLOR;

        removeElementByName(selectedIngredients, clickedIngredient.textContent);

    }
    else {
        clickedIngredient.style.background = SELECTED_BACKGROUND_COLOR;
        clickedIngredient.style.color = SELECTED_TEXT_COLOR;

        selectedIngredients.push(clickedIngredient.textContent);
    }
}

function displayIngredients(clickedProduct) {
    const clickedProductIngredients = clickedProduct.children[1].children;
    let currentIngredients = document.getElementById("currentIngredientsList");
    let ownedIngredients = document.getElementById("ownedIngredientsList");
    let notOwnedIngredients = document.getElementById("notOwnedIngredientsList");

    ownedIngredients.innerHTML = "";
    notOwnedIngredients.innerHTML = currentIngredients.innerHTML;

    for (let ingredient of clickedProductIngredients) {
        let ingredientElement = document.getElementById(ingredient.textContent);

        ingredientElement.style.background = SELECTED_BACKGROUND_COLOR;
        ingredientElement.style.color = SELECTED_TEXT_COLOR;

        selectedIngredients.push(ingredientElement.textContent);

        let clonedIngredient = ingredientElement.cloneNode(true);
        clonedIngredient.style.backgroundColor = UNSELECTED_BACKGROUND_COLOR;
        clonedIngredient.style.color = UNSELECTED_TEXT_COLOR;
        ownedIngredients.appendChild(clonedIngredient);

        let owned = notOwnedIngredients.querySelector("#" + ingredient.textContent);
        notOwnedIngredients.removeChild(owned);
    }

    console.log(selectedIngredients);
}

function searchProduct() {
    const productFilter = document.getElementById("productFilter");
    const inputName = productFilter.value.toUpperCase();
    let currentProducts = document.getElementById("currentProductsList");


    if (inputName === "") {
        for (let product of currentProducts.children) {
            product.style.display = "flex";
        }

        for (let product of currentProducts.children) {
            product.style.display = "flex";
        }

        for (let product of currentProducts.children) {
            product.style.display = "flex";
        }
    }
    else {
        for (let product of currentProducts.children) {
            const upperProduct = product.textContent.toUpperCase();

            if (!upperProduct.includes(inputName)) {
                product.style.display = "none";
            }
        }
    }
}

function searchIngredient() {
    const ingredientFilter = document.getElementById("ingredientFilter");
    const inputName = ingredientFilter.value.toUpperCase();
    let currentIngredients = document.getElementById("currentIngredientsList");
    let ownedIngredients = document.getElementById("ownedIngredientsList");
    let notOwnedIngredients = document.getElementById("notOwnedIngredientsList");

    if (inputName === "") {
        for (let ingredient of currentIngredients.children) {
            ingredient.style.display = "flex";
        }

        for (let ingredient of ownedIngredients.children) {
            ingredient.style.display = "flex";
        }

        for (let ingredient of notOwnedIngredients.children) {
            ingredient.style.display = "flex";
        }
    }
    else {
        for (let ingredient of currentIngredients.children) {
            const upperIngredient = ingredient.textContent.toUpperCase();

            if (!upperIngredient.includes(inputName)) {
                ingredient.style.display = "none";
            }
        }

        for (let ingredient of ownedIngredients.children) {
            const lowerIngredient = ingredient.textContent.toUpperCase();

            if (!lowerIngredient.includes(inputName)) {
                ingredient.style.display = "none";
            }
        }

        for (let ingredient of notOwnedIngredients.children) {
            const lowerIngredient = ingredient.textContent.toUpperCase();

            if (!lowerIngredient.includes(inputName)) {
                ingredient.style.display = "none";
            }
        }
    }
}
function selectAllIngredients() {
    let currentIngredients = document.getElementById("currentIngredientsList");
    let ownedIngredients = document.getElementById("ownedIngredientsList");
    let notOwnedIngredients = document.getElementById("notOwnedIngredientsList");

    currentIngredients.style.display = "block";
    ownedIngredients.style.display = "none";
    notOwnedIngredients.style.display = "none";

    const allIngredients = document.getElementById("currentIngredientsList");

    allIngredients.style.display = "block";

    let allIngredientsButton = document.getElementById("allIngredientsButton");
    let ownedIngredientsButton = document.getElementById("ownedIngredientsButton");
    let notOwnedIngredientsButton = document.getElementById("notOwnedIngredientsButton");

    allIngredientsButton.style.backgroundColor = SELECTED_BUTTON_BACKGROUND_COLOR;
    ownedIngredientsButton.style.backgroundColor = UNSELECTED_BUTTON_BACKGROUND_COLOR;
    notOwnedIngredientsButton.style.backgroundColor = UNSELECTED_BUTTON_BACKGROUND_COLOR;

    //displayIngredients();
    //selectedIngredients = [];
}

function selectOwnedIngredients() {
    let allIngredientsButton = document.getElementById("allIngredientsButton");
    let ownedIngredientsButton = document.getElementById("ownedIngredientsButton");
    let notOwnedIngredientsButton = document.getElementById("notOwnedIngredientsButton");

    allIngredientsButton.style.backgroundColor = UNSELECTED_BUTTON_BACKGROUND_COLOR;
    ownedIngredientsButton.style.backgroundColor = SELECTED_BUTTON_BACKGROUND_COLOR;
    notOwnedIngredientsButton.style.backgroundColor = UNSELECTED_BUTTON_BACKGROUND_COLOR;


    let currentIngredients = document.getElementById("currentIngredientsList");
    let ownedIngredients = document.getElementById("ownedIngredientsList");
    let notOwnedIngredients = document.getElementById("notOwnedIngredientsList");

    currentIngredients.style.display = "none";
    ownedIngredients.style.display = "block";
    notOwnedIngredients.style.display = "none";

    //selectedIngredients = [];
}

function selectNotOwnedIngredients() {
    let allIngredientsButton = document.getElementById("allIngredientsButton");
    let ownedIngredientsButton = document.getElementById("ownedIngredientsButton");
    let notOwnedIngredientsButton = document.getElementById("notOwnedIngredientsButton");

    allIngredientsButton.style.backgroundColor = UNSELECTED_BUTTON_BACKGROUND_COLOR;
    ownedIngredientsButton.style.backgroundColor = UNSELECTED_BUTTON_BACKGROUND_COLOR;
    notOwnedIngredientsButton.style.backgroundColor = SELECTED_BUTTON_BACKGROUND_COLOR;


    let currentIngredients = document.getElementById("currentIngredientsList");
    let ownedIngredients = document.getElementById("ownedIngredientsList");
    let notOwnedIngredients = document.getElementById("notOwnedIngredientsList");

    currentIngredients.style.display = "none";
    ownedIngredients.style.display = "none";
    notOwnedIngredients.style.display = "block";

    //selectedIngredients = [];
}

function setNewIngredientName(newIngredient) {
    if (newIngredient.children[0]) {
        newIngredient.id = newIngredient.children[0].value;
    }
}

function addNewIngredient() {
    let currentIngredients = document.getElementById("currentIngredientsList");

    let newIngredient = document.createElement("li");
    newIngredient.className = "currentIngredient";
    newIngredient.onclick = function () { selectIngredient(newIngredient) };

    added_ingredients.push(newIngredient);

    let newIngredientInput = document.createElement("input");
    newIngredientInput.className = "newIngredientInput";
    newIngredientInput.style.fontSize = "20px";
    newIngredientInput.style.fontWeight = "700";
    newIngredientInput.style.color = "rgb(20, 20, 51)";
    newIngredientInput.onchange = setNewIngredientName(newIngredient);
    newIngredientInput.onclick = function (event) { event.stopPropagation(); };

    newIngredient.appendChild(newIngredientInput);
    currentIngredients.appendChild(newIngredient);
}

function saveIngredients() {
    let data = {
        added_ingredients: []
    };

    for (let ingredientElement of added_ingredients) {
        data.added_ingredients.push(ingredientElement.children[0].value);
    }

    const url = "/admin/add/ingredient";
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

    setTimeout(() => { window.location.href = "/admin"; }, 100);
}

function deleteIngredients() {
    let data = {
        deleted_ingredients: selectedIngredients
    }

    const url = "/admin/delete/ingredient";
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

    setTimeout(() => { window.location.href = "/admin"; }, 100);
}

function displayAllergens(clickedProduct) {
    const clickedProductAllergens = clickedProduct.children[2].children;
    let currentAllergens = document.getElementById("currentAllergensList");
    let ownedAllergens = document.getElementById("ownedAllergensList");
    let notOwnedAllergens = document.getElementById("notOwnedAllergensList");

    ownedAllergens.innerHTML = "";
    notOwnedAllergens.innerHTML = currentAllergens.innerHTML;

    for (let allergen of clickedProductAllergens) {
        let allergenElement = document.getElementById(allergen.textContent);

        allergenElement.style.background = SELECTED_BACKGROUND_COLOR;
        allergenElement.style.color = SELECTED_TEXT_COLOR;

        selectedAllergens.push(allergenElement.textContent);

        let clonedAllergen = allergenElement.cloneNode(true);
        clonedAllergen.style.backgroundColor = UNSELECTED_BACKGROUND_COLOR;
        clonedAllergen.style.color = UNSELECTED_TEXT_COLOR;
        ownedAllergens.appendChild(clonedAllergen);

        let owned = notOwnedAllergens.querySelector("#" + allergen.textContent);
        notOwnedAllergens.removeChild(owned);
    }
}

function searchAllergen() {
    const allergenFilter = document.getElementById("allergenFilter");
    const inputName = allergenFilter.value.toUpperCase();
    let currentAllergens = document.getElementById("currentAllergensList");
    let ownedAllergens = document.getElementById("ownedAllergensList");
    let notOwnedAllergens = document.getElementById("notOwnedAllergensList");

    if (inputName === "") {
        for (let allergen of currentAllergens.children) {
            allergen.style.display = "flex";
        }

        for (let allergen of ownedAllergens.children) {
            allergen.style.display = "flex";
        }

        for (let allergen of notOwnedAllergens.children) {
            allergen.style.display = "flex";
        }
    }
    else {
        for (let allergen of currentAllergens.children) {
            const upperIngredient = allergen.textContent.toUpperCase();

            if (!upperIngredient.includes(inputName)) {
                allergen.style.display = "none";
            }
        }

        for (let allergen of ownedAllergens.children) {
            const upperAllergens = allergen.textContent.toUpperCase();

            if (!upperAllergens.includes(inputName)) {
                allergen.style.display = "none";
            }
        }

        for (let allergen of notOwnedAllergens.children) {
            const upperAllergens = allergen.textContent.toUpperCase();

            if (!upperAllergens.includes(inputName)) {
                allergen.style.display = "none";
            }
        }
    }
}

function selectAllAllergens() {
    let currentAllergens = document.getElementById("currentAllergensList");
    let ownedAllergens = document.getElementById("ownedAllergensList");
    let notOwnedAllergens = document.getElementById("notOwnedAllergensList");

    currentAllergens.style.display = "block";
    ownedAllergens.style.display = "none";
    notOwnedAllergens.style.display = "none";

    const allAllergens = document.getElementById("currentAllergensList");

    allAllergens.style.display = "block";

    let allAllergensButton = document.getElementById("allAllergensButton");
    let ownedAllergensButton = document.getElementById("ownedAllergensButton");
    let notOwnedAllergensButton = document.getElementById("notOwnedAllergensButton");

    allAllergensButton.style.backgroundColor = SELECTED_BUTTON_BACKGROUND_COLOR;
    ownedAllergensButton.style.backgroundColor = UNSELECTED_BUTTON_BACKGROUND_COLOR;
    notOwnedAllergensButton.style.backgroundColor = UNSELECTED_BUTTON_BACKGROUND_COLOR;

    //displayAllergens();
    //selectedAllergens = [];
}

function selectOwnedAllergens() {
    let allAllergensButton = document.getElementById("allAllergensButton");
    let ownedAllergensButton = document.getElementById("ownedAllergensButton");
    let notOwnedAllergensButton = document.getElementById("notOwnedAllergensButton");

    allAllergensButton.style.backgroundColor = UNSELECTED_BUTTON_BACKGROUND_COLOR;
    ownedAllergensButton.style.backgroundColor = SELECTED_BUTTON_BACKGROUND_COLOR;
    notOwnedAllergensButton.style.backgroundColor = UNSELECTED_BUTTON_BACKGROUND_COLOR;


    let currentAllergens = document.getElementById("currentAllergensList");
    let ownedAllergens = document.getElementById("ownedAllergensList");
    let notOwnedAllergens = document.getElementById("notOwnedAllergensList");

    currentAllergens.style.display = "none";
    ownedAllergens.style.display = "block";
    notOwnedAllergens.style.display = "none";

    //selectedAllergens = [];
}

function selectNotOwnedAllergens() {
    let allAllergensButton = document.getElementById("allAllergensButton");
    let ownedAllergensButton = document.getElementById("ownedAllergensButton");
    let notOwnedAllergensButton = document.getElementById("notOwnedAllergensButton");

    allAllergensButton.style.backgroundColor = UNSELECTED_BUTTON_BACKGROUND_COLOR;
    ownedAllergensButton.style.backgroundColor = UNSELECTED_BUTTON_BACKGROUND_COLOR;
    notOwnedAllergensButton.style.backgroundColor = SELECTED_BUTTON_BACKGROUND_COLOR;


    let currentAllergens = document.getElementById("currentAllergensList");
    let ownedAllergens = document.getElementById("ownedAllergensList");
    let notOwnedAllergens = document.getElementById("notOwnedAllergensList");

    currentAllergens.style.display = "none";
    ownedAllergens.style.display = "none";
    notOwnedAllergens.style.display = "block";

    //selectedAllergens = [];
}

function setNewAllergenName(newAllergen) {
    if (newAllergen.children[0]) {
        newAllergen.id = newAllergen.children[0].value;
    }
}

function selectAllergen(clickedAllergen) {
    if (clickedAllergen.style.backgroundColor === SELECTED_BACKGROUND_COLOR) {
        clickedAllergen.style.background = UNSELECTED_BACKGROUND_COLOR;
        clickedAllergen.style.color = UNSELECTED_TEXT_COLOR;

        removeElementByName(selectedAllergens, clickedAllergen.textContent);

    }
    else {
        clickedAllergen.style.background = SELECTED_BACKGROUND_COLOR;
        clickedAllergen.style.color = SELECTED_TEXT_COLOR;

        selectedAllergens.push(clickedAllergen.textContent);
    }
}

function addNewAllergen() {
    let currentAllergens = document.getElementById("currentAllergensList");

    let newAllergen = document.createElement("li");
    newAllergen.className = "currentAllergen";
    newAllergen.onclick = function () { selectIngredient(newAllergen) };

    added_allergens.push(newAllergen);

    let newAllergenInput = document.createElement("input");
    newAllergenInput.className = "newAllergenInput";
    newAllergenInput.style.fontSize = "20px";
    newAllergenInput.style.fontWeight = "700";
    newAllergenInput.style.color = "rgb(20, 20, 51)";
    newAllergenInput.onchange = setNewAllergenName(newAllergen);
    newAllergenInput.onclick = function (event) { event.stopPropagation(); };

    newAllergen.appendChild(newAllergenInput);
    currentAllergens.appendChild(newAllergen);
}

function saveAllergens() {
    let data = {
        added_allergens: []
    };

    for (let allergenElement of added_allergens) {
        data.added_allergens.push(allergenElement.children[0].value);
    }

    const url = "/admin/add/allergen";
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

    setTimeout(() => { window.location.href = "/admin"; }, 100);
}

function deleteAllergens() {
    let data = {
        deleted_allergens: selectedAllergens
    }

    const url = "/admin/delete/allergen";
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

    setTimeout(() => { window.location.href = "/admin"; }, 100);
}


function saveRelationships() {
    let data = {
        selected_product: selectedProduct,
        selected_ingredients: selectedIngredients,
        selected_allergens: selectedAllergens
    };

    console.log(data);

    const url = "/admin/add/relationship";
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

    setTimeout(() => { window.location.href = "/admin"; }, 100);
}