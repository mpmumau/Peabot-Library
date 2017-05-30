function click_button()
{
    var el = document.getElementById("the_button");
    el.style.background = "red";

    var xml = new XMLHttpRequest();
    

    xml.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            console.log(JSON.parse(this.responseText)); 
        }
    };
    xml.open("POST", "http://ML_DEVNET_PIBOT:9976/event/walk", true);
    xml.setRequestHeader("Content-Type", "application/json;charset=UTF-8");
    xml.send(JSON.stringify({ cycles: 3, duration: 0.35 }));
}