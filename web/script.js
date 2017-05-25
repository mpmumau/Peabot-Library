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
    xml.open("GET", "http://ML_DEVNET_PIBOT:9976/whazupbitch", true);
    xml.send();    
}