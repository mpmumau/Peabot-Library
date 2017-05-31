function walk_button()
{
    var xml = new XMLHttpRequest();
    
    xml.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            console.log(JSON.parse(this.responseText)); 
        }
    };
    xml.open("POST", "http://ML_DEVNET_PIBOT:9976/event/walkx", true);
    xml.setRequestHeader("Content-Type", "application/json;charset=UTF-8");
    xml.send(JSON.stringify({ cycles: 13, duration: 0.75, reverse: true }));
}

function turn_button()
{
    var xml = new XMLHttpRequest();
    
    xml.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            console.log(JSON.parse(this.responseText)); 
        }
    };
    xml.open("POST", "http://ML_DEVNET_PIBOT:9976/event/turn", true);
    xml.setRequestHeader("Content-Type", "application/json;charset=UTF-8");
    xml.send(JSON.stringify({ cycles: 30 }));
}

function elevate_button()
{
    var xml = new XMLHttpRequest();
    
    xml.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            console.log(JSON.parse(this.responseText)); 
        }
    };
    xml.open("POST", "http://ML_DEVNET_PIBOT:9976/event/elevate", true);
    xml.setRequestHeader("Content-Type", "application/json;charset=UTF-8");
    xml.send(JSON.stringify({ reverse: true, duration: 3.0 }));
}

function extend_button()
{
    var xml = new XMLHttpRequest();
    
    xml.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            console.log(JSON.parse(this.responseText)); 
        }
    };
    xml.open("POST", "http://ML_DEVNET_PIBOT:9976/event/extend", true);
    xml.setRequestHeader("Content-Type", "application/json;charset=UTF-8");
    xml.send(JSON.stringify({ reverse: true, duration: 3.0 }));
}

function reset_button()
{
    var xml = new XMLHttpRequest();
    
    xml.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            console.log(JSON.parse(this.responseText)); 
        }
    };
    xml.open("POST", "http://ML_DEVNET_PIBOT:9976/event/reset", true);
    xml.setRequestHeader("Content-Type", "application/json;charset=UTF-8");
    xml.send();
}

function halt_button()
{
    var xml = new XMLHttpRequest();
    
    xml.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            console.log(JSON.parse(this.responseText)); 
        }
    };
    xml.open("POST", "http://ML_DEVNET_PIBOT:9976/event/halt", true);
    xml.setRequestHeader("Content-Type", "application/json;charset=UTF-8");
    xml.send();
}

function set_usd()
{
    setInterval(usd_setter, 500);
}

function usd_setter()
{
    var xml = new XMLHttpRequest();
    
    xml.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            var data = JSON.parse(this.responseText);
            document.getElementById("usd").innerHTML = data.distance;
        }
    };
    xml.open("GET", "http://ML_DEVNET_PIBOT:9976/usd/get", true);
    xml.send();
}

window.onload = set_usd;