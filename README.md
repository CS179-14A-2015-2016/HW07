HW07 — WORCHBOUND 1.5 (deadline: 2015-11-10)
===

FIX YOUR CODE.

Make sure to:

* Use proper game timing (utilize either fixed frame rate or flexible frame rate).
* Use proper integration methods, even the simple euler is fine.
* Use a vector class (reuse and improve the one you did on HW05)
* Use floating-point math (A LOT OF PEOPLE ARE USING INTEGERS WTF.)
* Remember: Wind and gravity are FORCES. They affect ACCELERATION.
* Use proper collision detection between box and circle (we discussed this in class).
* Measure time in SECONDS (not in MILLISECONDS).
* Use `<random>`  for random sources (use this for [reference](http://en.cppreference.com/w/cpp/numeric/random)). Don't use `rand()`
* MAKE YOUR CODE READABLE.

Here's the previous specs:
---

Write a basic "clone" of Gunbound/Scorch/Worms.

* 2-player turn-based game.
* No movement yet.
* Each player take turns shooting an artillery round in an attempt to hit the other player.
* The active player sets an angle and power to shoot the round.
* Wind effects is present and changes per turn at random.
* The rounds follow the projectile motion.
* Model each player as a rectangle.
* Model the artillery rounds as circles (get it? hahah).

References:
* Scorch: https://en.wikipedia.org/wiki/Scorched_Earth_(video_game)
* Worms: https://en.wikipedia.org/wiki/Worms_(series)
* Gunbound: https://www.youtube.com/watch?v=ThW5g8VUcts
