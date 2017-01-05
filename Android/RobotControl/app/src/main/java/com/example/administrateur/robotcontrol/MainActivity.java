package com.example.administrateur.robotcontrol;

import android.content.ActivityNotFoundException;
import android.content.Intent;
import android.graphics.Color;
import android.graphics.Point;
import android.media.MediaPlayer;
import android.net.Uri;
import android.os.AsyncTask;
import android.os.Handler;
import android.speech.RecognizerIntent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.Gravity;
import android.view.MotionEvent;
import android.view.View;
import android.widget.CheckBox;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.SeekBar;
import android.widget.Switch;
import android.widget.TableRow;
import android.widget.TextView;
import android.widget.Toast;
import android.widget.ToggleButton;
import android.widget.VideoView;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Locale;

import de.tavendo.autobahn.WebSocketConnection;
import de.tavendo.autobahn.WebSocketException;
import de.tavendo.autobahn.WebSocketHandler;
import edu.cmu.pocketsphinx.Assets;
import edu.cmu.pocketsphinx.Hypothesis;
import edu.cmu.pocketsphinx.RecognitionListener;
import edu.cmu.pocketsphinx.SpeechRecognizer;

import static android.widget.Toast.makeText;
import static edu.cmu.pocketsphinx.SpeechRecognizerSetup.defaultSetup;

public class MainActivity extends AppCompatActivity implements RecognitionListener
{

    private static final String TAG = "Socket info";
    private final WebSocketConnection mConnection = new WebSocketConnection();
    private static final String ip = "192.168.0.35:8082";
    private static final String streamAddress = "http://192.168.0.35:8081";

    private VocalCommandAnalyser vocalCommandAnalyser;
    private VoiceSynthesizer voiceSynthesizer;

    private ImageButton leftButton;
    private ImageButton rightButton;
    private ImageButton vocalButton;
    private ToggleButton moveToggleButton;
    private Switch reverseSwitch;
    private ImageButton turnCameraLeftButton;
    private ImageButton turnCameraRightButton;
    private SeekBar speedSeekBar;
    private ImageView pointerImageView;
    private LinearLayout controlPanelLayout;
    private CheckBox speedLockCheckBox;
    private VideoView videoView;
    private TableRow controlTableRow;

    private MjpegView mv;
    private static final int MENU_QUIT = 1;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        //Initialisation de la connexion avec le WebSocketServer
        startConnexion();

        vocalCommandAnalyser = new VocalCommandAnalyser();
        voiceSynthesizer = new VoiceSynthesizer(this);

        //Récupération des éléments graphiques
        /*leftButton = (ImageButton) findViewById(R.id.leftButton);
        rightButton = (ImageButton) findViewById(R.id.rightButton);*/
        turnCameraLeftButton = (ImageButton) findViewById(R.id.turnCameraLeftButton);
        turnCameraRightButton = (ImageButton) findViewById(R.id.turnCameraRightButton);
        vocalButton = (ImageButton) findViewById(R.id.vocalButton);
        controlTableRow = (TableRow) findViewById(R.id.controlTableRow);
        /*moveToggleButton = (ToggleButton) findViewById(R.id.moveToggleButton);
        reverseSwitch = (Switch) findViewById(R.id.reverseSwitch);*/
        speedSeekBar = (SeekBar) findViewById(R.id.speedSeekBar);
        pointerImageView = (ImageView) findViewById(R.id.pointerImageView);
        controlPanelLayout = (LinearLayout) findViewById(R.id.controlPanelLayout);
        speedLockCheckBox = (CheckBox) findViewById(R.id.speedLockcheckBox);
        /*videoView = (VideoView) findViewById(R.id.videoView);

        videoView.setVideoURI(Uri.parse("http://192.168.0.35:8081"));
        videoView.setOnPreparedListener(new MediaPlayer.OnPreparedListener() {
            @Override
            public void onPrepared(MediaPlayer mp) {
                videoView.start();
            }
        });*/

        //Lecture du stream vidéo
        mv = new MjpegView(this);
        TableRow videoViewTableRow = (TableRow) findViewById(R.id.videoViewTableRow);
        TableRow.LayoutParams params = new TableRow.LayoutParams();
        params.width = TableRow.LayoutParams.MATCH_PARENT;
        params.gravity = Gravity.CENTER;
        params.weight = 1;
        mv.setLayoutParams(params);
        mv.setMinimumWidth(videoViewTableRow.getWidth());
        videoViewTableRow.addView(mv);

        new StreamReader().execute();


        //Commandes vocales
        vocalButton.setOnClickListener(new View.OnClickListener() {

           @Override
           public void onClick(View v) {
               promptSpeechInput();
           }
       });

        //Centrage du pointeur de contrôle
        pointerImageView.post(new Runnable() {
            @Override
            public void run() {
                LinearLayout.LayoutParams params = (LinearLayout.LayoutParams) pointerImageView.getLayoutParams();
                params.leftMargin = (controlPanelLayout.getWidth()/2)-(pointerImageView.getWidth()/2);
                params.topMargin = (controlPanelLayout.getHeight()/2)-(pointerImageView.getHeight()/2);
                pointerImageView.setLayoutParams(params);
            }
        });

        pointerImageView.setOnTouchListener(new View.OnTouchListener() {

            private int largerCircleRay = 115;
            private String state = "stopped";
            private String wheelsState = "none";
            private long lastSpeedChangeTime = 0;

            @Override
            public boolean onTouch(View v, MotionEvent event) {
                int action = event.getActionMasked();
                int controlPanelLayoutHorizontalStart = (int) controlPanelLayout.getX();
                int controlPanelLayoutVerticalStart = (int) controlTableRow.getY();
                int controlPanelLayoutWidth = controlPanelLayout.getWidth();
                int controlPanelLayoutHeight = controlPanelLayout.getHeight();
                int pointerImageViewWidth = pointerImageView.getWidth();
                int pointerImageViewHeight = pointerImageView.getHeight();
                LinearLayout.LayoutParams mParams;

                switch(action) {
                    case MotionEvent.ACTION_DOWN:
                        if (!mConnection.isConnected()) { startConnexion(); }
                        if (mConnection.isConnected()) { mConnection.sendTextMessage("movement;move"); }
                        state = "moving";
                        break;
                    case MotionEvent.ACTION_MOVE:
                        //On prépare les nouvelle coordonnées de l'image
                        int x = (int)(event.getRawX()-40);
                        int y = (int)(event.getRawY()- 150);

                        //On transforme les coordonnées pour les représentées dans un repère orthonormée dont le point (0,0) est le centre du ControlPanelLayout
                        double xOrtho = x-(pointerImageViewWidth/2)-(controlPanelLayoutWidth/2)-controlPanelLayoutHorizontalStart;
                        double yOrtho = -y+(pointerImageViewHeight/2)+(controlPanelLayoutHeight/2)+controlPanelLayoutVerticalStart;

                        //On vérifie que l'image ne sort pas de la zone de contrôle
                        if (!pointOnDisk(xOrtho, yOrtho, 0, 0, largerCircleRay)) {
                            try {
                                Point p = nearestPointOnCircle(xOrtho, yOrtho, 0, 0, largerCircleRay);
                                x = p.x - (pointerImageViewWidth/2) + (controlPanelLayoutWidth/2);
                                y = -p.y - (pointerImageViewHeight/2) + (controlPanelLayoutHeight/2);
                                xOrtho = p.x;
                                yOrtho = p.y;
                            } catch (Exception e) {
                                e.printStackTrace();
                            }
                        }
                        else {
                            x -= controlPanelLayoutHorizontalStart;
                            y -= controlPanelLayoutVerticalStart;
                        }
                        //On envoie des ordres à la raspberry si nécessaire
                        if (xOrtho >= 0) { xOrtho -= (pointerImageViewWidth/2); }
                        else { xOrtho += (pointerImageViewWidth/2); }
                        if (yOrtho >= 0) { yOrtho -= (pointerImageViewHeight/2); }
                        else { yOrtho += (pointerImageViewHeight/2); }
                        sendOrders(xOrtho,yOrtho);
                        //On change les coordonnées de l'image
                        LinearLayout.LayoutParams layoutParams = (LinearLayout.LayoutParams) pointerImageView.getLayoutParams();
                        layoutParams.leftMargin = x;
                        layoutParams.topMargin = y;
                        pointerImageView.setLayoutParams(layoutParams);
                        break;
                    case MotionEvent.ACTION_UP:
                        if (!mConnection.isConnected()) { startConnexion(); }
                        if (mConnection.isConnected()) { mConnection.sendTextMessage("movement;stop"); }
                        state = "stopped";
                        //On repositionne le pointeur visuel au centre de l'interface de contrôle
                        mParams = (LinearLayout.LayoutParams) pointerImageView.getLayoutParams();
                        mParams.leftMargin = (controlPanelLayout.getWidth()/2)-(pointerImageViewWidth/2);
                        mParams.topMargin = (controlPanelLayout.getHeight()/2)-(pointerImageViewHeight/2);
                        pointerImageView.setLayoutParams(mParams);
                        break;
                    case MotionEvent.ACTION_CANCEL:
                        break;
                }

                return true;
            }

            /**
             * La fonction pointOnDisk permet de savoir si un point est situé sur un disque
             * @param x Abscisse du point dont on veut savoir s'il est sur le disque
             * @param y Ordonnée du point dont on veut savoir s'il est sur le disque
             * @param cx Abscisse du centre du disque
             * @param cy Ordonnée du centre du disque
             * @param r Rayon du disque
             * @return true si le point (x,y) est situé sur le disque de centre (cx,cy) et de rayon r, false sinon
             */
            private boolean pointOnDisk(double x, double y, double cx, double cy, double r)
            {
                return Math.pow(x - cx, 2) + Math.pow(y - cy, 2) <= Math.pow(r, 2);
            }

            /**
             * La fonction nearestPointOnCircle permet de connaître le point situé sur un cercle étant le plus proche d'un point donné
             * @param x Abscisse du point pour lequel on veut connaître le point le plus proche sur le cercle
             * @param y Ordonnée du point pour lequel on veut connaître le point le plus proche sur le cercle
             * @param cx Abscisse du centre du cercle
             * @param cy Ordonnée du centre du cercle
             * @param r Rayon du cercle
             * @return Le point situé sur le cercle de centre (cx,cy) de rayon r qui est le plus proche du point (x,y)
             * @throws Exception Exception si le point et le centre du cercle sont confondus car dans ce cas il y a une infinité de possibilités
             */
            private Point nearestPointOnCircle(double x, double y, double cx, double cy, double r) throws Exception {
                if (x == cx && y == cy) { throw new Exception("Error : Infinite possibilities. A point which is different of the center of the circle is needed"); }
                Point p = new Point();
                //Calcul de a et b dans l'équation de la droite passant par les points (x,y) et (cx,cy)
                double a = (cy-y)/(cx-x);
                double b = y-a*x;
                //A partir de l'équation de la droite et de l'équation du cercle ( (x-cx)²+(y-cy)²=r² ) on obtient le polynôme du 2nd dégré suivant :
                // Az² + Bz + C = 0
                // Avec : A = 1+a² ; B = 2(a(b-cy)-cx) et C = cx²+(b-cy)²+r²
                double A = 1+Math.pow(a,2);
                double B = 2*(a*(b-cy)-cx);
                double C = Math.pow(cx,2)+Math.pow(b-cy,2)-Math.pow(r,2);
                //On calcul delta = B² - 4AC
                double delta = Math.pow(B, 2)-4*A*C;
                //Résolution
                if (delta > 0) {
                    double s1 = (-B-Math.sqrt(delta))/(2*A);
                    double s2 = (-B+Math.sqrt(delta))/(2*A);
                    if (s1 > 0 && x > 0) {
                        p.x = (int) s1;
                        p.y = (int) (a*s1+b);
                    }
                    else if (s1 < 0 && x < 0) {
                        p.x = (int) s1;
                        p.y = (int) (a*s1+b);
                    }
                    else {
                        p.x = (int) s2;
                        p.y = (int) (a*s2+b);
                    }
                }
                //Il n'y a qu'un seul cas pour lequel les deux points d'intersection de la droite passant par le centre du cercle et du cercle ont la
                //même abscisse, c'est quand x vaut 0. Le cas delta < 0 n'a pas besoin d'être traité car il ne peut pas se produire si l'on utilise
                //seulement des nombres réels
                else {
                    p.x = (int) (-B/(2*A));
                    if (y > 0) { p.y = (int) r; }
                    else { p.y = (int) -r; }
                }
                return p;
            }

            private void sendOrders(double x, double y)
            {
                if (y >= 0 && state != "forward") {
                    if (!mConnection.isConnected()) { startConnexion(); }
                    if (mConnection.isConnected()) { mConnection.sendTextMessage("movement;forward"); }
                    state = "forward";
                }
                else if (y <= 0 && state != "reverse") {
                    if (!mConnection.isConnected()) { startConnexion(); }
                    if (mConnection.isConnected()) { mConnection.sendTextMessage("movement;reverse"); }
                    state = "reverse";
                }
                if (!speedLockCheckBox.isChecked()) {
                    long time = System.currentTimeMillis();
                    if (time-lastSpeedChangeTime > 1000) {
                        lastSpeedChangeTime = time;
                        double distance = distanceBetween(0, 0, x, y);
                        int speed = (int) (distance / (largerCircleRay / 100));
                        if (!mConnection.isConnected()) { startConnexion(); }
                        if (mConnection.isConnected()) { mConnection.sendTextMessage("movement;speed;" + Integer.toString(speed)); }
                    }
                }
                double angle = angle(x,y);
                Log.i("ANGLE","x="+x+" y="+y+" et angle="+angle);
                String wheelsRotation = "";
                boolean stateChanged = false;
                if (angle >= 0 && angle < 36 && wheelsState.compareTo("rightForward") != 0) {
                    stateChanged = true;
                    wheelsState = "rightForward";
                    wheelsRotation = "movement;wheels;rightForward";
                }
                else if (angle >= 36 && angle < 72 && wheelsState.compareTo("rightTop") != 0) {
                    stateChanged = true;
                    wheelsState = "rightTop";
                    wheelsRotation = "movement;wheels;rightTop";
                }
                else if (angle >= 72 && angle < 108 && wheelsState.compareTo("straightforward") != 0) {
                    stateChanged = true;
                    wheelsState = "straightforward";
                    wheelsRotation = "movement;wheels;straightforward";
                }
                else if (angle >= 108 && angle < 144 && wheelsState.compareTo("leftTop") != 0) {
                    stateChanged = true;
                    wheelsState = "leftTop";
                    wheelsRotation = "movement;wheels;leftTop";
                }
                else if (angle >= 144 && angle < 180 && wheelsState.compareTo("leftForward") != 0) {
                    stateChanged = true;
                    wheelsState = "leftForward";
                    wheelsRotation = "movement;wheels;leftForward";
                }
                else if (angle >= 180 && angle < 216 && wheelsState.compareTo("leftReverse") != 0) {
                    stateChanged = true;
                    wheelsState = "leftReverse";
                    wheelsRotation = "movement;wheels;leftReverse";
                }
                else if (angle >= 216 && angle < 252 && wheelsState.compareTo("leftBottom") != 0) {
                    stateChanged = true;
                    wheelsState = "leftBottom";
                    wheelsRotation = "movement;wheels;leftBottom";
                }
                else if (angle >= 252 && angle < 288 && wheelsState.compareTo("straightforwardReverse") != 0) {
                    stateChanged = true;
                    wheelsState = "straightforwardReverse";
                    wheelsRotation = "movement;wheels;straightforwardReverse";
                }
                else if (angle >= 288 && angle < 324 && wheelsState.compareTo("rightBottom") != 0) {
                    stateChanged = true;
                    wheelsState = "rightBottom";
                    wheelsRotation = "movement;wheels;rightBottom";
                }
                else if (angle >= 324 && angle < 360 && wheelsState.compareTo("rightReverse") != 0) {
                    stateChanged = true;
                    wheelsState = "rightReverse";
                    wheelsRotation = "movement;wheels;rightReverse";
                }
                if (!mConnection.isConnected()) { startConnexion(); }
                if (mConnection.isConnected() && stateChanged) { mConnection.sendTextMessage(wheelsRotation); }
            }

            private double distanceBetween(double x1, double y1, double x2, double y2)
            {
                return Math.sqrt(Math.pow(x2-x1,2)+Math.pow(y2-y1,2));
            }

            private double angle(double x, double y)
            {
                if (y == 0) { return 0; }
                double d = distanceBetween(0,0,x,y);
                double d2 = distanceBetween(0,0,x,0);
                double cos = d2/d;
                double angle = toDegrees(Math.acos(cos));
                if (x < 0 && y >= 0) { angle = 180 - angle; }
                else if (x < 0 && y < 0) { angle = 180 + angle; }
                else if (x >= 0 && y < 0) { angle = 360 - angle; }
                return angle;
            }

            private double toDegrees (double angle)
            {
                return angle * (180 / Math.PI);
            }

            private double toRadians (double angle)
            {
                return angle * (Math.PI / 180);
            }
        });

        turnCameraRightButton.setOnTouchListener(new View.OnTouchListener() {

            @Override
            public boolean onTouch(View v, MotionEvent event) {
                if (event.getAction() == MotionEvent.ACTION_DOWN) {
                    if (!mConnection.isConnected()) { startConnexion(); }
                    if (mConnection.isConnected()) { mConnection.sendTextMessage("camera;turnRight"); }
                }
                else if (event.getAction() == MotionEvent.ACTION_UP) {
                    if (!mConnection.isConnected()) { startConnexion(); }
                    if (mConnection.isConnected()) { mConnection.sendTextMessage("camera;stopRotation"); }
                }
                return false;
            }
        });

        turnCameraLeftButton.setOnTouchListener(new View.OnTouchListener() {

            /*private Handler handler;

            Runnable sendMessage = new Runnable() {
                @Override
                public void run() {
                    if (!mConnection.isConnected()) { startConnexion(); }
                    mConnection.sendTextMessage("camera;turnLeft");
                    handler.postDelayed(this, 500);
                }
            };*/

            @Override
            public boolean onTouch(View v, MotionEvent event) {
                if (event.getAction() == MotionEvent.ACTION_DOWN) {
                    if (!mConnection.isConnected()) { startConnexion(); }
                    if (mConnection.isConnected()) { mConnection.sendTextMessage("camera;turnLeft"); }
                }
                else if (event.getAction() == MotionEvent.ACTION_UP) {
                    if (!mConnection.isConnected()) { startConnexion(); }
                    if (mConnection.isConnected()) { mConnection.sendTextMessage("camera;stopRotation"); }
                }
                return false;
            }
        });

        /*//Lorsque l'on appuie sur rightButton on envoie à la raspberry l'ordre tourner à droite
        rightButton.setOnTouchListener(new View.OnTouchListener() {

            private Handler handler;

            Runnable sendMessage = new Runnable() {
                @Override
                public void run() {
                    if (!mConnection.isConnected()) { startConnexion(); }
                    mConnection.sendTextMessage("movement;right");
                    handler.postDelayed(this, 500);
                }
            };

            @Override
            public boolean onTouch(View v, MotionEvent event) {
                if (event.getAction() == MotionEvent.ACTION_DOWN) {
                    if (handler != null) { return true; }
                    handler = new Handler();
                    handler.postDelayed(sendMessage, 100);
                }
                else if (event.getAction() == MotionEvent.ACTION_UP) {
                    if (handler == null) return true;
                    handler.removeCallbacks(sendMessage);
                    handler = null;
                }
                return false;
            }
        });

        //Lorsque l'on appuie sur leftButton on envoie à la raspberry l'ordre tourner à gauche
        leftButton.setOnTouchListener(new View.OnTouchListener() {

            private Handler handler;

            Runnable sendMessage = new Runnable() {
                @Override
                public void run() {
                    if (!mConnection.isConnected()) { startConnexion(); }
                    mConnection.sendTextMessage("movement;left");
                    handler.postDelayed(this, 500);
                }
            };

            @Override
            public boolean onTouch(View v, MotionEvent event) {
                if (event.getAction() == MotionEvent.ACTION_DOWN) {
                    if (handler != null) { return true; }
                    handler = new Handler();
                    handler.postDelayed(sendMessage, 100);
                }
                else if (event.getAction() == MotionEvent.ACTION_UP) {
                    if (handler == null) return true;
                    handler.removeCallbacks(sendMessage);
                    handler = null;
                }
                return false;
            }
        });

        //Lors d'un clic sur moveToggleButton on envoie à la raspberry l'ordre bouger ou l'ordre d'arrêter le robot
        moveToggleButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v)
            {
                if (!mConnection.isConnected()) { startConnexion(); }
                if (moveToggleButton.isChecked()) {
                    mConnection.sendTextMessage("movement;move");
                }
                else {
                    mConnection.sendTextMessage("movement;stop");
                }
            }
        });

        //Lors d'un clic sur le switch on envoie à la raspberry l'ordre de se déplacer en marche avant ou en marche arrière
        reverseSwitch.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v)
            {
                if (!mConnection.isConnected()) { startConnexion(); }
                if (reverseSwitch.isChecked()) {
                    mConnection.sendTextMessage("movement;reverse");
                }
                else {
                    mConnection.sendTextMessage("movement;forward");
                }
            }
        });*/

        //Lorsque l'on change la valeur de la seekBar réglant la vitesse on envoie à la raspberry un ordre de changement de vitesse
        speedSeekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                String progressStringVal = Integer.toString(progress);
                if (!mConnection.isConnected()) { startConnexion(); }
                if (mConnection.isConnected()) { mConnection.sendTextMessage("movement;speed;"+progressStringVal); }
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                int progress = seekBar.getProgress();
                String progressStringVal = Integer.toString(progress);
                if (!mConnection.isConnected()) { startConnexion(); }
                if (mConnection.isConnected()) {  mConnection.sendTextMessage("movement;speed;"+progressStringVal); }
            }
        });


        /*****************************************************************************************/
        /*************************** TEST RECO VOCALE SPHINX *************************************/
        /******************************************************************************************/

        new AsyncTask<Void, Void, Exception>() {
            @Override
            protected Exception doInBackground(Void... params) {
                try {
                    Assets assets = new Assets(MainActivity.this);
                    File assetDir = assets.syncAssets();
                    setupRecognizer(assetDir);
                } catch (IOException e) {
                    e.printStackTrace();
                    return e;
                }
                return null;
            }

            @Override
            protected void onPostExecute(Exception result) {
                if (result != null) {

                } else {
                    switchSearch(KWS_SEARCH);
                }
            }
        }.execute();

        /*********************************************************************************************/
        /***************************** FIN TEST RECO VOCALE SPHINX ***********************************/
        /********************************************************************************************/

    }

    private void startConnexion()
    {

        final String wsuri = "ws://"+ip+"?id=0";

        try {
            mConnection.connect(wsuri, new WebSocketHandler() {

                @Override
                public void onOpen() {
                    Log.d(TAG, "Status: Connected to " + wsuri);
                }

                @Override
                public void onTextMessage(String payload) {
                    Log.d(TAG, "Got echo: " + payload);
                }

                @Override
                public void onClose(int code, String reason) {
                    Log.d(TAG, "Connection lost.");
                }
            });
        } catch (WebSocketException e) {

            Log.d(TAG, e.toString());
        }
    }

    public class StreamReader extends AsyncTask<String, Void, MjpegInputStream> {

        @Override
        protected MjpegInputStream doInBackground(String... params) {
            return MjpegInputStream.read(streamAddress);
        }

        protected void onPostExecute(MjpegInputStream result) {
            mv.setSource(result);
            mv.setDisplayMode(MjpegView.SIZE_BEST_FIT);
            mv.showFps(false);
        }
    }

    /***********************************************************************/
    /******************* RECONNAISSANCE VOCALE ****************************/
    /*********************************************************************/

    private final int REQ_CODE_SPEECH_INPUT = 100;

    /**
     * Showing google speech input dialog
     * */
    private void promptSpeechInput() {
        Intent intent = new Intent(RecognizerIntent.ACTION_RECOGNIZE_SPEECH);
        intent.putExtra(RecognizerIntent.EXTRA_LANGUAGE_MODEL,
                RecognizerIntent.LANGUAGE_MODEL_FREE_FORM);
        intent.putExtra(RecognizerIntent.EXTRA_LANGUAGE, Locale.getDefault());
        intent.putExtra(RecognizerIntent.EXTRA_PREFER_OFFLINE, true);
        intent.putExtra(RecognizerIntent.EXTRA_PROMPT,
                getString(R.string.speech_prompt));
        try {
            startActivityForResult(intent, REQ_CODE_SPEECH_INPUT);
        } catch (ActivityNotFoundException a) {
            Toast.makeText(getApplicationContext(),
                    getString(R.string.speech_not_supported),
                    Toast.LENGTH_SHORT).show();
        }
    }

    /**
     * Receiving speech input
     * */
    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);

        switch (requestCode) {
            case REQ_CODE_SPEECH_INPUT: {
                if (resultCode == RESULT_OK && null != data) {
                    ArrayList<String> result = data.getStringArrayListExtra(RecognizerIntent.EXTRA_RESULTS);
                    if (!mConnection.isConnected()) { startConnexion(); }
                    if(!vocalCommandAnalyser.vocalCommandProcessing(mConnection, result.get(0))) {
                        Toast.makeText(getApplicationContext(),
                                "Requête invalide, veuillez répéter.",
                                Toast.LENGTH_SHORT).show();
                    }
                }
            }
        }
    }


    /*******************************************************************************************/
    /**************** TEST RECO VOCALE SPHINX **************************************************/
    /********************************************************************************************/
    /* Named searches allow to quickly reconfigure the decoder */
    private static final String KWS_SEARCH = "wakeup";
    private static final String SEARCH = "recherche";

    /* Keyword we are looking for to activate menu */
    private static final String KEYPHRASE = "activation contrôle vocal";
    private static final String STOPPHRASE = "fin";

    private SpeechRecognizer recognizer;

    @Override
    public void onDestroy() {
        super.onDestroy();
        recognizer.cancel();
        recognizer.shutdown();
    }

    /**
     * In partial result we get quick updates about current hypothesis. In
     * keyword spotting mode we can react here, in other modes we need to wait
     * for final result in onResult.
     */
    @Override
    public void onPartialResult(Hypothesis hypothesis) {
        if (hypothesis == null)
            return;

        String text = hypothesis.getHypstr();
        if (text.equals(KEYPHRASE)) {
            switchSearch(SEARCH);
        }
        else if (recognizer.getSearchName().compareTo(SEARCH) == 0) {
            if (text.equals(STOPPHRASE)) { switchSearch(KWS_SEARCH); }
            else { switchSearch(SEARCH); }
            /*Toast.makeText(getApplicationContext(),
                    text,
                    Toast.LENGTH_SHORT).show();
            Log.i("TEST TOAST", "TEST TOAST");*/
        }
    }

    /**
     * This callback is called when we stop the recognizer.
     */
    @Override
    public void onResult(Hypothesis hypothesis) {
        if (hypothesis != null) {
            String text = hypothesis.getHypstr();
            Toast.makeText(getApplicationContext(), text, Toast.LENGTH_SHORT).show();
            if (text.compareTo(KEYPHRASE) == 0) {
                voiceSynthesizer.speak("Contrôle vocal activé");
            }
            else if (text.compareTo(STOPPHRASE) == 0) {
                voiceSynthesizer.speak("Contrôle vocal désactivé");
            }
            else { vocalCommandAnalyser.vocalCommandProcessing(mConnection, text); }
        }
    }

    @Override
    public void onBeginningOfSpeech() {
    }

    /**
     * We stop recognizer here to get a final result
     */
    @Override
    public void onEndOfSpeech() {
        if (!recognizer.getSearchName().equals(KWS_SEARCH))
            switchSearch(KWS_SEARCH);
    }

    private void switchSearch(String searchName) {
        recognizer.stop();

        // If we are not spotting, start listening with timeout (10000 ms or 10 seconds).
        //if (searchName.equals(KWS_SEARCH))
            recognizer.startListening(searchName);
        /*else
            recognizer.startListening(searchName, 10000);*/

    }

    private void setupRecognizer(File assetsDir) throws IOException {
        // The recognizer can be configured to perform multiple searches
        // of different kind and switch between them

        recognizer = defaultSetup()
                .setAcousticModel(new File(assetsDir, "cmusphinx-fr-ptm-5.2"))
                .setDictionary(new File(assetsDir, "fr.dict"))

                // To disable logging of raw audio comment out this call (takes a lot of space on the device)
                .setRawLogDir(assetsDir)

                // Threshold to tune for keyphrase to balance between false alarms and misses
                .setKeywordThreshold(1e-45f)

                // Use context-independent phonetic search, context-dependent is too slow for mobile
                .setBoolean("-allphone_ci", true)

                .getRecognizer();
        recognizer.addListener(this);

        /** In your application you might not need to add all those searches.
         * They are added here for demonstration. You can leave just one.
         */

        // Create keyword-activation search.
        recognizer.addKeyphraseSearch(KWS_SEARCH, KEYPHRASE);


        File languageModel = new File(assetsDir, "fr.gram");
        recognizer.addGrammarSearch(SEARCH, languageModel);
    }

    @Override
    public void onError(Exception error) {
        Toast.makeText(getApplicationContext(),
                error.getMessage(),
                Toast.LENGTH_SHORT).show();
    }

    @Override
    public void onTimeout() {
        switchSearch(KWS_SEARCH);
    }


    /*********************************************************************************************/
    /***************** FIN TEST RECO VOCALE SPHINX **********************************************/
    /*********************************************************************************************/



}

