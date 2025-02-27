//
//  snake.js
//  ~~~~~~~~
//
//  Modified from: https://codepen.io/cluk3/pen/OMqaBY
var debug = {
    x: [],
    y: [],
    food: []
  };
  (function(debug) {
    var canvas      = document.getElementById("game_canvas");
    var msg         = document.getElementById('msg');
    var currScore   = document.getElementById('current-score');
    var bestScore   = document.getElementById('best-score');
    var ctx         = canvas.getContext("2d");
    var gameOn      = false;
    var speed       = 2.5;
    var endGame     = new Event('endGame');
    var scores      = createScores();
    var interval;
    var anim;
    var block_size  = 20;
    var board_spaces_x = canvas.width / block_size;
    var board_spaces_y = canvas.height / block_size;
    document.addEventListener('endGame', function endGameHandler(e) {
      msg.innerHTML = 'Game Over! Press <em>Space</em> to play a new game'
      gameOn = false;
      window.cancelAnimationFrame(anim);
      clearInterval(interval);
      scores.stop();
    }, false);
    document.addEventListener("keydown", function spaceHandler(e) {
      if (e.keyCode === 32) {
        e.preventDefault();
        if (!gameOn) {
          msg.innerHTML = 'Use WASD or arrow keys to play. Good Luck!'
          gameOn = true;
          game();
        }
      }
    }, false);
    function game() {
      var snake = createSnake(speed);
      var food = createFood(canvas.width, canvas.height, "#ff7e67");
      var nextDir = '';
      var fps = 0;
      
      scores.start();
      document.addEventListener('keydown', arrowsHandler, false);
      function arrowsHandler(e) {
        switch (e.keyCode) {
          case 38: // W
          case 87: // up
            e.preventDefault();
            nextDir = 'up';
            break;
          case 65: // A
          case 37: // left
            e.preventDefault();
            nextDir = 'left';
            break;
          case 83: // S
          case 40: // down
            e.preventDefault();
            nextDir = 'down';
            break;
          case 68: // D
          case 39: // right
            e.preventDefault();
            nextDir = 'right';
            break;
          default:
        }
      }
      function play() {
        fps++;
        ctx.clearRect(0, 0, canvas.width, canvas.height);
        if ((snake.x[0] % block_size === 0) && (snake.y[0] % block_size === 0)) {
          snake.update();
          snake.changeDir(nextDir);
        }
        snake.move();
        if (snake.isEating(food.x, food.y)) {
          food.spawn(snake.x, snake.y);
          snake.grow();
          scores.incr(1);
        }
        food.draw(ctx);
        snake.draw(ctx);
        if (snake.collides(canvas.width, canvas.height)) {
          document.dispatchEvent(endGame);
        } else {
          anim = window.requestAnimationFrame(play);
        }
        debug.x = snake.x;
        debug.y = snake.y;
        debug.food = [food.x, food.y];
      }
      
      interval = setInterval(function() {
        fps = 0;
      }, 1000);
      food.spawn(snake.x, snake.y);
      anim = window.requestAnimationFrame(play);
    }
    function createFood(width, height, color) {
      return {
        x: 0,
        y: 0,
        // spawn the food in a random free space
        spawn: function(posX, posY) {
          var matrix = [],
            free = [],
            x, y, foodPos, i, len;
          for (i = 0; i < board_spaces_y; i++) {
            matrix[i] = Array(board_spaces_x).fill(0);
          }
          for (i = 0, len = posX.length; i < len; i++) {
            x = Math.floor(posX[i] / board_spaces_x);
            y = Math.floor(posY[i] / board_spaces_y);
            matrix[y][x] = 1;
          }
          for (i = 0; i < board_spaces_y; i++) {
            for (var j = 0; j < board_spaces_x; j++) {
              if (!matrix[i][j])
                free.push([i, j]);
            }
          }
          foodPos = Math.floor(Math.random() * free.length);
          this.x = free[foodPos][1] * board_spaces_x;
          this.y = free[foodPos][0] * board_spaces_y;
        },
        draw: function(ctx) {
          ctx.beginPath();
          ctx.arc(this.x + (block_size/2), this.y + (block_size/2), (block_size/2), 0, Math.PI * 2);
          ctx.fillStyle = color;
          ctx.fill();
          ctx.closePath();
        }
      }
    }
    function createSnake(speed) {
      
      function unshiftAndCut(arr) {
        var result = arr.slice();
        return result.map(function(el, i) {
          if (!i)
            return arr[0];
          return arr[i - 1];
        });
      }
      
      return {
        x: [0],
        y: [0],
        dx: [speed],
        dy: [0],
        changeDir: function(nextDir) {
          if (this.dx[0] && nextDir === 'up') {
            this.dx[0] = 0;
            this.dy[0] = -speed;
          }
          if (this.dx[0] && nextDir === 'down') {
            this.dx[0] = 0;
            this.dy[0] = speed;
          }
          if (this.dy[0] && nextDir === 'right') {
            this.dx[0] = speed;
            this.dy[0] = 0;
          }
          if (this.dy[0] && nextDir === 'left') {
            this.dx[0] = -speed;
            this.dy[0] = 0;
          }
        },
        draw: function(ctx) {
          for (var i = 0, len = this.x.length; i < len; i++) {
            ctx.beginPath();
            ctx.rect(this.x[i], this.y[i], block_size, block_size);
            ctx.fillStyle = '#6a8caf';
            ctx.strokeStyle = '#5a7c9f';
            ctx.fill();
            ctx.stroke();
            ctx.closePath();
          }
        },
        update: function() {
          this.dx = unshiftAndCut(this.dx);
          this.dy = unshiftAndCut(this.dy);
        },
        grow: function() {
          var lastX = this.x[this.x.length - 1];
          var lastY = this.y[this.y.length - 1];
          var lastDX = this.dx[this.dx.length - 1]
          var lastDY = this.dy[this.dy.length - 1]
          this.x.push(lastX - (Math.sign(lastDX) * block_size));
          this.y.push(lastY - (Math.sign(lastDY) * block_size));
          this.dx.push(lastDX);
          this.dy.push(lastDY);
        },
        isEating: function(foodX, foodY) {
          return this.x[0] === foodX && this.y[0] === foodY;
        },
        move: function() {
          for (var i = 0, len = this.x.length; i < len; i++) {
            this.x[i] += this.dx[i];
            this.y[i] += this.dy[i];
          }
        },
        collides: function(width, height) {
          var x = this.x[0],
            y = this.y[0];
          // Check collision with the wall
          if (x < 0 || x + block_size > width || y < 0 || y + block_size > height)
            return true;
          // Check collision with itself
          for (var i = 1, len = this.x.length; i < len; i++) {
            if (x === this.x[i] && y === this.y[i])
              return true;
          }
          return false;
        }
      }
    }
    
    function createScores() {
      var best = 0,
        score;
      return {
        start: function() {
          score = 0;
          currScore.innerText = '0';
        },
        incr: function(n) {
          score += n;
          currScore.innerText = '' + score;
        },
        stop: async function() {
          if (score > best) {
            best = score;
            bestScore.innerText = '' + best;
          }
          let shouldUpdateLeaderboard = await fetch(
            "/leaderboard/best/" + score, 
            { method: "GET"}
          ).then(res => res.status === 200)
          .catch(e => console.log(e, 'error fetching leaderboard/best'))

          if (shouldUpdateLeaderboard) {
            var name = prompt("High score! If you'd like, enter your name here to be featured on our leaderboard:", "");
            if (name != null) {
              fetch("/leaderboard", {
                method: "POST",
                body: `${name}\n${score}\n`
              }).catch(e => console.log(e, 'error posting to leaderboard'));
            }
          }
        }
      }
    }
  })(debug)