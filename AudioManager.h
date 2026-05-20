#pragma once

#include <SFML/Audio.hpp>
#include <string>
#include <iostream>

using namespace std;

class AudioManager {
private:
    sf::Music music;
    
    sf::SoundBuffer moveBuffer;
    sf::SoundBuffer shootBuffer;
    sf::SoundBuffer explosionBuffer;
    sf::SoundBuffer buttonBuffer;
    sf::SoundBuffer powerUpBuffer;

    sf::Sound moveSound;
    sf::Sound shootSound;
    sf::Sound explosionSound;
    sf::Sound buttonSound;
    sf::Sound powerUpSound;

    bool muted;
    bool loaded;

public:
    AudioManager();

    bool load();
    void playMusic();
    void stopMove();
    void playMove();
    void playShoot();
    void playExplosion();
    void playButton();
    void playPowerUp();

    void toggleMute();
    bool isMuted();
};