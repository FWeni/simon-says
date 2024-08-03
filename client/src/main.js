let url = 'httplocalhost:8080/';

function overlayOff(){
    document.querySelector(".overlay").style.display = "none";
}
function overlayOn(){
    document.querySelector(".overlay").style.display = "block";
}

function landingPg(msg) {
    document.getElementById("welcome").innerHTML = msg;
}

let count = 5; 

const timer = setInterval(function() {
    count--;
    console.log(count);
    if (count === 0) {
        clearInterval(timer);
        console.log("go");
        
    }
}, 80)


async function gameServerLoading() {
    /* On load , get a message that the server is actually running */
    await fetch(url)
        .then(response => response.json())
        .then(data => landingPg(data.message))
        .catch(error => console.error('Error:', error));
}

async function start() {

    let user_input = {
        input : []
    }
    await fetch(url, { 
        method : "POST",
        headers: {
            "Content-Type" : "application.json"
        },
        body : JSON.stringify(user_input)
    })
        .then(response => 
        {
            if(!response.ok) {
                throw new Error("Server response in not ok!") // set a clear error
            }
            response.json();
        })
        .then(data => 
        {
            console.log(data);
        }
        )
        .catch(error => console.error('Error:', error));

    


}

