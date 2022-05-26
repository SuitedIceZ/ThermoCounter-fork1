
 // Your web app's Firebase configuration
 // For Firebase JS SDK v7.20.0 and later, measurementId is optional
 const firebaseConfig = {
   apiKey: "AIzaSyCX5lkPx_8AXYw15jWerggs8u32PgKz6X4",
   authDomain: "thermocounter.firebaseapp.com",
   projectId: "thermocounter",
   storageBucket: "thermocounter.appspot.com",
   messagingSenderId: "944854017559",
   appId: "1:944854017559:web:e5b1977f4d511d54d45e52",
   measurementId: "G-HL32D1KH21"
 };

 // Initialize Firebase
 firebase.initializeApp(firebaseConfig);
 const db = firebase.firestore();

var add_button = document.getElementById("add_button");

async function init(){ //add_button.onclick = function(){
    let info = document.getElementById("info");
    //body_main.innerHTML = '';
    var room_list = await db.collection('1').get();
    for(const doc of room_list.docs){
            var room_name = doc.data().room;
            var room_capacity = doc.data().capacity;
            var room_amount = doc.data().amount;
            let h2 = document.createElement("h2");
            h2.innerHTML = room_name;
            let h3 = document.createElement("h3");
            h3.className ="avilable";
            h3.innerHTML = room_amount + '/' + room_capacity;
            let div = document.createElement("div");
            div.className = "card";
            div.appendChild(h2);
            div.appendChild(h3);
            console.log( "room name " +  room_name);
            info.appendChild(div);
        }
}
init();
