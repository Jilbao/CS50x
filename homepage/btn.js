document.addEventListener("DOMContentLoaded",function(){

    function changeBG(btn, fb, span){
        document.getElementById("aside").style.backgroundImage = 'url("./img/'+ btn.textContent +'.jpg")';
        span.textContent = btn.textContent ;
        fb.textContent = "Check TITLE";
    };
    let chill = document.getElementById("btnchill");
    let curious = document.getElementById("btncurious");
    let hostile = document.getElementById("btnhostile");
    let feedback = document.getElementById("feedback");
    let btnspan = document.getElementById("btnspan");

    curious.addEventListener("click",  function() {changeBG(curious, feedback, btnspan)});
    hostile.addEventListener("click",  function() {changeBG(hostile, feedback, btnspan)});
    chill.addEventListener("click", function() {changeBG(chill, feedback, btnspan)});



});