document.addEventListener("DOMContentLoaded",function(){
    // Part-1
    var btn = document.getElementsByTagName("button");
    var btn_correct = document.getElementById("correct");
    var feedback1 = document.getElementById("feedback1");

    //correct answer button
    btn_correct.addEventListener('click', function(){
        btn_correct.style.backgroundColor = 'green';
        btn_correct.style.color = 'white';
        feedback1.textContent = "Correct !";
        feedback1.style.color = "green";
    });

    //wrong answer button (-2 because 1 from index 0 other one is correct button)
    for (let i = 0; i < (btn.length-2); i++)
    {
        btn[i].addEventListener("click", function(){
            btn[i].style.backgroundColor = "red";
            btn[i].style.color = "white";
            feedback1.textContent ="Incorrect !";
            feedback1.style.color = "red";       
        });
    }
    // Part-2 
    var checkbtn = document.getElementById("checkbtn");
    var input = document.querySelector("input");
    var feedback2 = document.getElementById("feedback2");

    checkbtn.addEventListener("click", function(){
        if (input.value === "Switzerland" || input.value === "switzerland" ){
            input.style.backgroundColor = "green";
            input.style.color = "white";
            feedback2.textContent = "Correct !";
            feedback2.style.color = "green";
        }
        else {
            input.style.backgroundColor = "red";
            input.style.color = "white";
            feedback2.textContent = "Incorrect !";
            feedback2.style.color = "red";
        }
    });
});



