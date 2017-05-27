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
    xml.setRequestHeader("Content-Type", "application/json");

    var request_str = JSON.stringify({name:"John Rambo", time:"2pm"});
    var request_str_len = request_str.len;

    xml.setRequestHeader("Content-Length", request_str_len);

    xml.send(JSON.stringify({name:"John Rambo", time:"2pm"}));    
}