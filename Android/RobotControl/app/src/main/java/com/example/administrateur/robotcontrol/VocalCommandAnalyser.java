package com.example.administrateur.robotcontrol;

import java.util.ArrayList;

import de.tavendo.autobahn.WebSocketConnection;

/**
 * Created by ariel on 03/01/2017.
 */

public class VocalCommandAnalyser
{
    private ArrayList<String> forwardKeywords;
    private ArrayList<String> reverseKeywords;
    private ArrayList<String> moveKeywords;
    private ArrayList<String> stopKeywords;
    private ArrayList<String> turnCameraRightKeywords;
    private ArrayList<String> turnCameraLeftKeywords;
    private ArrayList<String> stopCameraRotationKeywords;
    private ArrayList<String> speedUpKeywords;
    private ArrayList<String> slowDownKeywords;
    private ArrayList<String> turnRightForwardKeywords;
    private ArrayList<String> turnLeftForwardKeywords;
    private ArrayList<String> turnRightReverseKeywords;
    private ArrayList<String> turnLeftReverseKeywords;

    public VocalCommandAnalyser()
    {
        forwardKeywords = new ArrayList<String>();
        forwardKeywords.add("avance");
        forwardKeywords.add("avancer");
        forwardKeywords.add("en avant");
        forwardKeywords.add("marche avant");

        reverseKeywords = new ArrayList<String>();
        reverseKeywords.add("recule");
        reverseKeywords.add("reculer");
        reverseKeywords.add("en arrière");
        reverseKeywords.add("marche arrière");

        moveKeywords = new ArrayList<String>();
        moveKeywords.add("démarre");
        moveKeywords.add("démarrer");

        stopKeywords = new ArrayList<String>();
        stopKeywords.add("stop");
        stopKeywords.add("arrête");
        stopKeywords.add("arrêter");

        turnCameraRightKeywords = new ArrayList<String>();
        turnCameraRightKeywords.add("tourne la caméra à droite");
        turnCameraRightKeywords.add("tourne la caméra vers la droite");
        turnCameraRightKeywords.add("tourne caméra vers la droite");
        turnCameraRightKeywords.add("tourne caméra à droite");
        turnCameraRightKeywords.add("tourne caméra droite");
        turnCameraRightKeywords.add("caméra droite");

        turnCameraLeftKeywords = new ArrayList<String>();
        turnCameraLeftKeywords.add("tourne la caméra à gauche");
        turnCameraLeftKeywords.add("tourne la caméra vers la gauche");
        turnCameraLeftKeywords.add("tourne caméra vers la gauche");
        turnCameraLeftKeywords.add("tourne caméra à gauche");
        turnCameraLeftKeywords.add("tourne caméra gauche");
        turnCameraLeftKeywords.add("caméra gauche");

        stopCameraRotationKeywords = new ArrayList<String>();
        stopCameraRotationKeywords.add("stop caméra");
        stopCameraRotationKeywords.add("stop caméra rotation");
        stopCameraRotationKeywords.add("arrête caméra");
        stopCameraRotationKeywords.add("arrêter caméra");
        stopCameraRotationKeywords.add("arrêt caméra");

        speedUpKeywords = new ArrayList<String>();
        speedUpKeywords.add("accélère");
        speedUpKeywords.add("accélèrer");
        speedUpKeywords.add("plus vite");

        slowDownKeywords = new ArrayList<String>();
        slowDownKeywords.add("ralentis");
        slowDownKeywords.add("ralentir");
        slowDownKeywords.add("moins vite");

        turnRightForwardKeywords = new ArrayList<String>();
        turnRightForwardKeywords.add("avance à droite");
        turnRightForwardKeywords.add("avance droite");
        turnRightForwardKeywords.add("avancer à droite");
        turnRightForwardKeywords.add("avancer droite");

        turnLeftForwardKeywords = new ArrayList<String>();
        turnLeftForwardKeywords.add("avance à gauche");
        turnLeftForwardKeywords.add("avance gauche");
        turnLeftForwardKeywords.add("avancer à gauche");
        turnLeftForwardKeywords.add("avancer gauche");

        turnRightReverseKeywords = new ArrayList<String>();
        turnRightReverseKeywords.add("recule à droite");
        turnRightReverseKeywords.add("recule droite");
        turnRightReverseKeywords.add("reculer à droite");
        turnRightReverseKeywords.add("reculer droite");

        turnLeftReverseKeywords = new ArrayList<String>();
        turnLeftReverseKeywords.add("recule à gauche");
        turnLeftReverseKeywords.add("recule gauche");
        turnLeftReverseKeywords.add("reculer à gauche");
        turnLeftReverseKeywords.add("reculer gauche");

    }

    public boolean vocalCommandProcessing(WebSocketConnection mConnection, String command)
    {
        command = command.toLowerCase();
        if (isInArrayList(forwardKeywords, command)) {
            if (mConnection.isConnected()) { mConnection.sendTextMessage("movement;forward"); }
            return true;
        }
        else if (isInArrayList(reverseKeywords, command)) {
            if (mConnection.isConnected()) { mConnection.sendTextMessage("movement;reverse"); }
            return true;
        }
        else if (isInArrayList(stopKeywords, command)) {
            if (mConnection.isConnected()) { mConnection.sendTextMessage("movement;stop"); }
            return true;
        }
        else if (isInArrayList(moveKeywords, command)) {
            if (mConnection.isConnected()) { mConnection.sendTextMessage("movement;move"); }
            return true;
        }
        else if (isInArrayList(turnCameraRightKeywords, command)) {
            if (mConnection.isConnected()) { mConnection.sendTextMessage("camera;turnRight"); }
            return true;
        }
        else if (isInArrayList(turnCameraLeftKeywords, command)) {
            if (mConnection.isConnected()) { mConnection.sendTextMessage("camera;turnLeft"); }
            return true;
        }
        else if (isInArrayList(stopCameraRotationKeywords, command)) {
            if (mConnection.isConnected()) { mConnection.sendTextMessage("camera;stopRotation"); }
            return true;
        }
        else if (isInArrayList(speedUpKeywords, command)) {
            if (mConnection.isConnected()) { mConnection.sendTextMessage("movement;speed;speedUp"); }
            return true;
        }
        else if (isInArrayList(slowDownKeywords, command)) {
            if (mConnection.isConnected()) { mConnection.sendTextMessage("movement;speed;slowDown"); }
            return true;
        }
        else if (isInArrayList(turnRightForwardKeywords, command)) {
            if (mConnection.isConnected()) { mConnection.sendTextMessage("movement;wheels;rightForward"); }
            return true;
        }
        else if (isInArrayList(turnLeftForwardKeywords, command)) {
            if (mConnection.isConnected()) { mConnection.sendTextMessage("movement;wheels;leftForward"); }
            return true;
        }
        else if (isInArrayList(turnRightReverseKeywords, command)) {
            if (mConnection.isConnected()) { mConnection.sendTextMessage("movement;wheels;rightReverse"); }
            return true;
        }
        else if (isInArrayList(turnLeftReverseKeywords, command)) {
            if (mConnection.isConnected()) { mConnection.sendTextMessage("movement;wheels;leftReverse"); }
            return true;
        }
        return false;
    }

    private boolean isInArrayList(ArrayList<String> arrayList, String command)
    {
        for (String s : arrayList) {
            if (s.compareTo(command) == 0) { return true; }
        }
        return false;
    }
}
