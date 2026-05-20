#include "AudioManager.h"

AudioManager::AudioManager()
    : moveSound(moveBuffer),
    shootSound(shootBuffer),
    explosionSound(explosionBuffer),
    buttonSound(buttonBuffer),
    powerUpSound(powerUpBuffer)
{
    muted = false;
    loaded = false;
}

bool AudioManager::load() {
    bool ok = true;

    if (!music.openFromFile("assets/music.ogg")) {
        cout << "Error loading music.ogg" << endl;
        ok = false;
    }

    if (!moveBuffer.loadFromFile("assets/move.wav")) {
        cout << "Error loading move.wav" << endl;
        ok = false;
    }

    if (!shootBuffer.loadFromFile("assets/shoot.wav")) {
        cout << "Error loading shoot.wav" << endl;
        ok = false;
    }

    if (!explosionBuffer.loadFromFile("assets/explosion.wav")) {
        cout << "Error loading explosion.wav" << endl;
        ok = false;
    }

    if (!buttonBuffer.loadFromFile("assets/button.wav")) {
        cout << "Error loading button.wav" << endl;
        ok = false;
    }

    if (!powerUpBuffer.loadFromFile("assets/powerup.wav")) {
        cout << "Error loading powerup.wav" << endl;
        ok = false;
    }

    music.setLooping(true);
    music.setVolume(35.0f);

    moveSound.setVolume(55.0f);
    shootSound.setVolume(70.0f);
    explosionSound.setVolume(80.0f);
    buttonSound.setVolume(55.0f);
    powerUpSound.setVolume(75.0f);

    loaded = ok;
    return ok;
}

void AudioManager::playMusic() {
    if (loaded && !muted) {
        music.play();
    }
}

void AudioManager::playMove() {
    if (loaded && !muted) {
        moveSound.stop();
        moveSound.play();
    }
}

void AudioManager::playShoot() {
    if (loaded && !muted) {
        shootSound.play();
    }
}

void AudioManager::playExplosion() {
    if (loaded && !muted) {
        explosionSound.play();
    }
}

void AudioManager::playButton() {
    if (loaded && !muted) {
        buttonSound.play();
    }
}

void AudioManager::playPowerUp() {
    if (loaded && !muted) {
        powerUpSound.play();
    }
}

void AudioManager::toggleMute() {
    muted = !muted;

    if (muted) {
        music.pause();
    }
    else {
        music.play();
    }
}

bool AudioManager::isMuted() {
    return muted;
}
void AudioManager::stopMove() {
    if (loaded) {
        moveSound.stop();
    }
}