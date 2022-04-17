var systemMode = document.getElementById("systemMode");
var ignore = document.getElementById("ignoreMode");

function getSystemMode() {
    fetch("/api/get/systemMode")
        .then((d) => {
            return d.json();
        })
        .then(function(d) {
            systemMode = d.systemMode;
            setTimeout(getSystemMode, 500);
        })
        .catch(function(err) {
            console.log(err);
            setTimeout(getSystemMode, 500);
        });
}
getSystemMode();

function getIgnore() {
    fetch("/api/get/ignoreMode")
        .then((d) => {
            return d.json();
        })
        .then(function(d) {
            ignoreMode = d.ignoreMode;
            setTimeout(getIgnore, 500);
        })
        .catch(function(err) {
            console.log(err);
            setTimeout(getIgnore, 500);
        });
}
getIgnore();

function sendSystemMode() {
    console.log("sendSystemMode button was clicked!");
    var data = {
        systemMode: systemMode.value,
    };
    var xhr = new XMLHttpRequest();
    var url = "/api/set/systemMode";
    xhr.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            // Typical action to be performed when the document is ready:
            if (xhr.responseText != null) {
                console.log(xhr.responseText);
            }
        }
    };
    xhr.open("POST", url, true);
    xhr.send(JSON.stringify(data));
};

function sendIgnore() {
    console.log("sendIgnore button was clicked!");
    var data = {
        ignoreMode: ignoreMode.value,
    };
    var xhr = new XMLHttpRequest();
    var url = "/api/set/ignoreMode";
    xhr.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            // Typical action to be performed when the document is ready:
            if (xhr.responseText != null) {
                console.log(xhr.responseText);
            }
        }
    };
    xhr.open("POST", url, true);
    xhr.send(JSON.stringify(data));
};
