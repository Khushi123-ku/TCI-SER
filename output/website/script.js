
function filterTables() {

    var search = document.getElementById("globalSearch");

    if (!search)
        return;

    var value = search.value.toLowerCase();

    var rows = document.querySelectorAll("tbody tr");

    rows.forEach(function(row) {

        var text = row.innerText.toLowerCase();

        if (text.indexOf(value) !== -1) {
            row.style.display = "";
        }
        else {
            row.style.display = "none";
        }

    });
}


function filterCompany() {

    var select = document.getElementById("companyFilter");

    if (!select)
        return;

    var company = select.value.toLowerCase();

    var rows = document.querySelectorAll("tbody tr");

    rows.forEach(function(row) {

        var text = row.innerText.toLowerCase();

        if (company === "" || text.indexOf(company) !== -1) {
            row.style.display = "";
        }
        else {
            row.style.display = "none";
        }

    });
}


function filterStatus() {

    var select = document.getElementById("statusFilter");

    if (!select)
        return;

    var status = select.value.toLowerCase();

    var rows = document.querySelectorAll("tbody tr");

    rows.forEach(function(row) {

        var text = row.innerText.toLowerCase();

        if (status === "" || text.indexOf(status) !== -1) {
            row.style.display = "";
        }
        else {
            row.style.display = "none";
        }

    });
}


function clearFilters() {

    var search = document.getElementById("globalSearch");
    var company = document.getElementById("companyFilter");
    var status = document.getElementById("statusFilter");

    if (search)
        search.value = "";

    if (company)
        company.value = "";

    if (status)
        status.value = "";

    var rows = document.querySelectorAll("tbody tr");

    rows.forEach(function(row) {
        row.style.display = "";
    });
}


function printReport() {
    window.print();
}


function applyAllFilters() {

    var searchElement = document.getElementById("globalSearch");
    var companyElement = document.getElementById("companyFilter");
    var statusElement = document.getElementById("statusFilter");

    var search = searchElement ? searchElement.value.toLowerCase() : "";
    var company = companyElement ? companyElement.value.toLowerCase() : "";
    var status = statusElement ? statusElement.value.toLowerCase() : "";

    var rows = document.querySelectorAll("tbody tr");

    rows.forEach(function(row) {

        var text = row.innerText.toLowerCase();

        var searchOK = text.indexOf(search) !== -1;
        var companyOK = company === "" || text.indexOf(company) !== -1;
        var statusOK = status === "" || text.indexOf(status) !== -1;

        row.style.display =
            (searchOK && companyOK && statusOK) ? "" : "none";
    });
}
