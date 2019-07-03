function ajax(url, handler) {
    var req = new XMLHttpRequest();
    if (!req) {
        alert('Browser not supported.');
        return;
    }

    req.onreadystatechange = function() {        
        var resp;        
        if (this.readyState === XMLHttpRequest.DONE) {           
            if (this.status === 200) {               
                resp = this.responseText;            
                handler(resp);            
            } else {               
                handler('Ajax error, status: ' + this.status);           
            }       
        }    
    };
    req.open('GET', url);
  req.send();
}