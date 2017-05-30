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
    xml.send(JSON.stringify({ cycles: 1, duration: 1.0 }));
}