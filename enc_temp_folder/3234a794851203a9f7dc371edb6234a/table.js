function changeToProductsTab() {
    const ProductsTab = document.getElementById("tab-buttonProducts");
    const TicketTab = document.getElementById("tab-buttonTicket");
    ProductsTab.style.visibility = 'visible';
    TicketTab.style.visibility = 'hidden';
    showPage('Box');
}

function changeToTicketTab() {
    const ProductsTab = document.getElementById("tab-buttonProducts");
    const TicketTab = document.getElementById("tab-buttonTicket");
    ProductsTab.style.visibility = 'hidden';
    TicketTab.style.visibility = 'visible';
    showPage('Ticket');
}


// Function to show the specified page
function showPage(pageId) {
    // Hide all pages
    const pages = document.querySelectorAll('.page');
    pages.forEach(page => {
        page.style.display = 'none';
    });

    // Show the selected page
    const selectedPage = document.getElementById(`${pageId}-page`);
    if (selectedPage) {
        selectedPage.style.display = 'block';
    }
}