# RockyMtnGameJam2020
Godzilla Unreal Engine (UE4) Project

Programming: Shepherd Dowling<br/>
Graphics & Animation: Brian<br/>

This project will stay under devlopment even though the game-jam finished.<br/>


    1. created start menu
    2. created game-over menu (showing winning player)
    3. created a timer (UE4 as far as I know doesn't have a count-down timer)
    4. Created an Animations object
        - Links Standard Animations
        - Links Animation Montage
        - Returns to the idle loop after the animation completes
    5. Created a Collision object. In UE4, players get what is called a Collision Capsule (looks like a pill). <br/>
       This is great for 3rd person games where you are a human but sucks if you are a dinosaur or oddly shaped<br/>
       character. I developed a class that will handle collisions for any size character.
    6. Created a Shared-Screen Movable camera. It works similar to Mortal Kombat. It moves left and right as <br/>
       the players move around but unlike Mortal Kombat which is a 2.5D game, the camera I made will move forward</br>
       and backward as well. Also, as the players move further apart, the spring-arm will extend and as they come</br>
       closer together, it will close the distance!