User = function(name, moves) {
  this.name = name;
  this.score = 0;
  this.moves = moves;
	this.moved = 0;
  this.add = function(distance) {
    this.score = +this.score+ (+distance);
  }
}
UserFactory = function() {
	this.user = []
	this.count = 0;
	this.currentUser = 0;
	this.current = 0;
	this.add = function(name, moves) {
		this.user.push(new User(name, moves));
		this.count += 1;
	}
	this.addScores = function(d){
		user = this.user[this.currentUser].add(d)
		user = this.user[this.currentUser]
   document.getElementById("user"+this.currentUser).innerHTML = user.score;

		this.current += 1;
		if (this.current >= user.moves) {
			this.current = 0;
			this.currentUser += 1;
			if (this.currentUser >= this.user.length)
				this.currentUser = 0;
		}

		document.getElementById("pgame").innerHTML = this.user[this.currentUser].name
	}
};
users = new UserFactory();
renderUsers = function(factory, src, uname, checks) {
	factory.add(uname, checks);
	li = document.createElement("li")
	li.innerHTML = uname;
//	src.appendChild(li);
}
renderTable = function(factory, src) {
	src.innerHTML = "";
	for (var i = 0; i < factory.count; i++) {
			li = document.createElement("tr");
			li.innerHTML = "<td><h2>"+ factory.user[i].name + " - </h2>" + "</td><td><div class=\"score-container\" id='"+"user"+i +"'>" + factory.user[i].score + "</div></td>";
		
			src.appendChild(li);
	}
}
function startGame() {
	renderUsers(users, document.getElementById('ulist'),document.getElementById('uname').value, +document.getElementById("checks").value); 
	renderTable(users, document.getElementById('contests'));
	document.getElementById("pgame").innerHTML = users.user[0].name;
}
document.getElementById("strg").onclick = function(){startGame()}
