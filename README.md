# Uno++
Uno, but written in C++

```
:::    ::: ::::    :::  ::::::::                              
:+:    :+: :+:+:   :+: :+:    :+:      :+:           :+:      
+:+    +:+ :+:+:+  +:+ +:+    +:+      +:+           +:+      
+#+    +:+ +#+ +:+ +#+ +#+    +:+ +#++:++#++:++ +#++:++#++:++ 
+#+    +#+ +#+  +#+#+# +#+    +#+      +#+           +#+      
#+#    #+# #+#   #+#+# #+#    #+#      #+#           #+#      
 ########  ###    ####  ########                              
```

---

Plays like the classic uno game.
The only modification is the removal of needing to shout "Uno!" when down to one card.

Includes:
- 108 cards
	- 1-9, two of each color
	- One zero of each color
	- Two skips, draw twos, and reverses of each color
	- Four wilds
	- Four wild draw fours
- Multiplayer gameplay (no bots, you need friends)
- Scoring of hands
- Challenging draw fours
- Reneging

---

Build command: (I didn't bother setting up CMake)

```bash
g++ -std=c++23 -O2 -s -DNDEBUG ./src/*.cpp -I./include -o ./bin/uno.exe
```