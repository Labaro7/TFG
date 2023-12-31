const selectTableTab = document.getElementById("tab-buttonSelectTable");
const currentTablesTab = document.getElementById("tab-buttonCurrentTables");
const selectTablePage = document.getElementById("select-table-page");
const currentTablesPage = document.getElementById("current-tables-page");

function changeToSelectTableTab() {
    selectTableTab.style.visibility = 'visible';
    currentTablesTab.style.visibility = 'hidden';
    showPage('select-table-page');
}

function changeToCurrentTablesTab() {
    selectTableTab.style.visibility = 'hidden';
    currentTablesTab.style.visibility = 'visible';
    showPage('current-tables-page');
}

// Function to show the specified page
function showPage(pageId) {
    // Hide all pages
    const pages = document.querySelectorAll('.page');
    pages.forEach(page => {
        page.style.display = 'none';
    });

    // Show the selected page
    const selectedPage = document.getElementById(pageId);
    if (selectedPage) {
        selectedPage.style.display = 'flex';
    }
}

// Function to append a number to the display
function appendNumber(number) {
    const display = document.getElementById('tableInput');
    display.value += number;
}

// Function to append a dot to the display
function appendDot() {
    const display = document.getElementById('tableInput');
    display.value += '.';
}

// Function to clear the display
function clearDisplay() {
    const display = document.getElementById('tableInput');
    display.value = '';
}

function goToTable(clickedTable) {
    const n_table = (clickedTable.children)[1].textContent.substring(1);
    console.log(n_table);

    let url = 'https://192.168.1.66:18080/table?tableInput=' + n_table;

    fetchData(url)
        .then(response => {
            if (!response.ok) {
                throw new Error('Network response was not ok');
            }

            return response.text();
        })
        .then(data => {
            console.log('Data received:', data);
        })
        .catch(error => {
            console.error('Fetch error:', error);
        });
}


