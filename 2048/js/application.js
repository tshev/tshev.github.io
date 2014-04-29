// Wait till the browser is ready to render the game (avoids glitches)
document.getElementById("startgame").onclick = function() {
  new GameManager(4, KeyboardInputManager, HTMLActuator, LocalScoreManager);
}
