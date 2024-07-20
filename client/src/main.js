function overlayOff(){
    document.querySelector(".overlay").style.display = "none";
}
function overlayOn(){
    document.querySelector(".overlay").style.display = "block";
}

function landingPg(msg) {
    document.getElementById("welcome").innerHTML = msg;
}

async function onFetch() {
    await fetch('http://localhost:8080/')
        .then(response => response.json())
        .then(data => landingPg(data.message))
        .catch(error => console.error('Error:', error));
}