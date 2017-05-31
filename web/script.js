function walk_button()
{
    var xml = new XMLHttpRequest();
    
    xml.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            console.log(JSON.parse(this.responseText)); 
        }
    };
    xml.open("POST", "http://ML_DEVNET_PIBOT:9976/event/walk", true);
    xml.setRequestHeader("Content-Type", "application/json;charset=UTF-8");
    xml.send(JSON.stringify({ cycles: 13, duration: 0.75, reverse: false }));
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
    xml.send(JSON.stringify({ cycles: 30, duration: 0.35 }));
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