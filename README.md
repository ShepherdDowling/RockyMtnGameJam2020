# RockyMtnGameJam2020
Godzilla Unreal Engine (UE4) Project

Programming: Shepherd Dowling<br/>
Graphics & Animation Assets: Brian<br/>

The purpose of this repo is to demonstrate easy-to-use widley usable UE4 libraries and other programming methods. 


    1. Handling events between menus, game-modes and levels are all done in C++
    2. Watch.h (simple) created a timer (UE4 as far as I know doesn't have a count-down timer)
    3. Animate.h (moderate) - Easy-to-use animation handler object
        - Links Standard Animations
        - Links Animation Montage
        - Returns to the idle loop after the animation completes
    4. CollisionHandler.h (Advanced) Created a Collision object to handle collisions for
       non-humanoid type characters. In UE4, players get what is called a Collision Capsule 
       (looks like a pill). This is great for 3rd person games where you are a human but sucks if 
       you are a dinosaur or oddly shaped character. I developed a class that will handle collisions
       for any size character.
    5. MobileCamera.h (moderate). It works similar to the Mortal Kombat camera system.
       It moves left and right as the players move around but unlike Mortal Kombat which is a 
       2.5D game, the camera I made will move forward and backward as well. Also, as the players 
       move further apart, the spring-arm will extend, and as they come closer together, 
       it will close the distance!
    6. DualHandle.h - (simple) This is used to hold an object with physics enabled and with locked
       location & rotation.

