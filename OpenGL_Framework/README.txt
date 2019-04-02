To implement FMod:
1. Move Fmod.dll & Fmod64.dll to OpenGL_Framework folder
2. Move FMOD folder to OpenGL_Framework folder
3. Move FModWrapper.h & cpp to OpenGL_Framework folder
4. Include the .h and .cpp files to Project Solution
5. Include FModWrapper.h in Game.h

To get test sound to work:
In game.h
1. Create Sound object (e.g Sound testSound;)
2. Create Channel object (e.g FMOD::Channel* testChannel;)
3. Create FMOD Vector object (e.g FMOD_VECTOR testPos;)

In game.cpp (void Game::initializeGame())
4. Load the sound file (e.g testSound.Load("media/test.wav");)
5. Turn the sound channel on (e.g testChannel = testSound.Play(true);)
6. Set the position for the sound vector (e.g testPos = {0.0f, 0.0f, 0.0f};)
