function toggleCheckbox(x) {
    var xhr = new XMLHttpRequest();
    xhr.onreadystatechange = function() {
        if(xhr.readyState == XMLHttpRequest.DONE) {
            console.log(xhr.responseText);     
            let arduino_message = xhr.responseText;
            let arduino_message_value = arduino_message.split(":")[1];
            arduino_message_value.split(",").forEach(function(item, idx){
                //console.log(item, idx);
                if(idx > 0){
                    let document_id = "M" + idx + "_value";
                    document.getElementById(document_id).innerHTML = item;
                }
                
                //console.log(document_id);
                //
            });

            //document.getElementById('M1_value').innerHTML = '1';
        }
    }
    xhr.timeout = 5000;
    xhr.open("GET", "/action?go=" + x, true);
    xhr.send();
}

//window.onload = document.getElementById("photo").src = window.location.href.slice(0, -1) + ":81/stream";
