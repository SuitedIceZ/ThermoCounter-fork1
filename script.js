
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

async function init(){ 
    let floors = ["floor1","floor2","floor3","floor4"];
    for(var i=0;i<4;i++){
        let room_list = await db.collection((i+1).toString()).get();
        let floor = document.getElementById(floors[i]);
        console.log(i+1 + ' ' + room_list.lenght);
        for(const doc of room_list.docs){
            let room_name = doc.data().room;
            let room_capacity = doc.data().capacity;
            let room_amount = doc.data().amount;
            let h2 = document.createElement("h2");
            h2.innerHTML = room_name;
            let h3 = document.createElement("h3");
            h3.id = room_name;
            if(room_capacity > room_amount) h3.className ="avilable";
            else h3.className ="full";
            let div = document.createElement("div");
            div.className = "card";
            h3.innerHTML = room_amount + '/' + room_capacity;
            div.appendChild(h2);
            div.appendChild(h3);
            console.log(room_name);
            floor.appendChild(div);
        }
    }
}
init();

//add room feature
add_button.onclick = async function(){
    let select = document.getElementById('input_floor');
    let input_floor = select.options[select.selectedIndex].value;
    let input_name = document.getElementById('input_name').value;
    let input_capacity = document.getElementById('input_capacity').value;
    console.log("want to add: " + input_name + ' ' + input_capacity);
    
    let room_list = await db.collection(input_floor.toString()).get();
    let isDocExist = false;
    var room_doc;
    var id;
    for(const doc of room_list.docs){
        if(doc.data().room == input_name){
            room_doc = doc;    
            id = doc.id;
            isDocExist = true;
        }
    }
    var amount_to_add = 0;
    if (isDocExist) { //if exist - delete
        amount_to_add = room_doc.data().amount;
        console.log("Document data:", room_doc.data());
        await db.collection(input_floor.toString()).doc(id).delete().then(() => {
            console.log("Document successfully deleted!");
        }).catch((error) => {
            console.error("Error removing document: ", error);
        });
    }  
    //create new room
    await db.collection(input_floor.toString()).add({
        amount: amount_to_add,
        room: input_name,
        capacity: input_capacity
    })
    .then((docRef) => {
        console.log("Document written with ID: ", docRef.id);
    })
     .catch((error) => {
        console.error("Error adding document: ", error);
    });
    console.log("No such document!");
    window.location.reload();
}

var delete_button = document.getElementById("delete_button");

//delete room feature
delete_button.onclick = async function(){
    let input_delete_name = document.getElementById('input_delete_name').value;
    console.log("want to delete: " + input_delete_name);
    
    for(let i=1;i<=4;i++){
        let room_list = await db.collection(i.toString()).get();
        for(const doc of room_list.docs){
            if(doc.data().room == input_delete_name){
                await db.collection(i.toString()).doc(doc.id).delete().then(() => {
                    console.log("Document successfully deleted!");
                }).catch((error) => {
                    console.error("Error removing document: ", error);
                });
            }
        }
    }
    window.location.reload();
}


//realtime data listener & updater
for(var i=1;i<=4;i++){
    db.collection(i.toString()).onSnapshot((querySnapshot) => {
        querySnapshot.forEach((doc) => {
            let cerrent_room = document.getElementById(doc.data().room);
            if(doc.data().capacity > doc.data().amount) cerrent_room.className ="avilable";
                else cerrent_room.className ="full";
            cerrent_room.innerHTML = doc.data().amount + '/' + doc.data().capacity;
            console.log(doc.data().room + ' ' + doc.data().amount)
        });
    });
}

