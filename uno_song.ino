/* Uno song demo */

#define SONG_BUTTON_PIN 4
#define SPEAKER_PIN     9

bool     song_playing = false;
uint32_t song_start_ms;
uint32_t song_next_note_ms;
int      song_current_note_index;

/*==============================================
 * Song Defines
 *=============================================*/
#define NOTE_REST 0
#define NOTE_D3  147
#define NOTE_C4  262
#define NOTE_Db4 277
#define NOTE_D4  294
#define NOTE_Eb4 311
#define NOTE_E4  330
#define NOTE_F4  340
#define NOTE_Gb4 370
#define NOTE_G4  392
#define NOTE_Ab4 413
#define NOTE_A4  440
#define NOTE_Bb4 466
#define NOTE_B4  493
#define NOTE_C5  523
#define NOTE_Db5 554
#define NOTE_D5  587
#define NOTE_Eb5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_Gb5 739
#define NOTE_G5  783
#define NOTE_Ab5 830
#define NOTE_A5  880
#define NOTE_Bb5 932
#define NOTE_B5  987
#define NOTE_C6  1046
#define NOTE_Db6 1109

typedef struct
{
  int freq; 
  int duration_ms;
} note_type;

typedef struct
{
  note_type *notes;
  int        num_notes;
} song_type;

note_type scale[]=
{
  {NOTE_C5, 1000},
  {NOTE_D5, 1000},
  {NOTE_E5, 1000},
  {NOTE_F5, 1000},
  {NOTE_G5, 1000},
  {NOTE_A5, 1000},
  {NOTE_B5, 1000},
  {NOTE_C6, 1000}
};

note_type groove[]=
{
  {NOTE_Ab4, 300},
  {NOTE_REST, 100},
  {NOTE_Gb5, 200},
  {NOTE_Eb5, 200},
  {NOTE_Ab5, 200},
  {NOTE_Gb5, 200},
  {NOTE_Eb5, 200},
  {NOTE_Db5, 200},
  {NOTE_REST,200},
  {NOTE_B5,  200},
  {NOTE_Ab5, 200},
  {NOTE_Db6, 200},
  {NOTE_B5,  200},
  {NOTE_Ab5, 200},
  {NOTE_Gb5, 200},
  {NOTE_Ab5, 200} 
};

note_type temple[] =
{
  {NOTE_A4, 100},
  {NOTE_REST,50},
  {NOTE_A4, 100},
  {NOTE_REST,50},
  {NOTE_E5, 100},
  {NOTE_REST,50},
  {NOTE_E5, 100},
  {NOTE_REST,50},
  {NOTE_A5, 100},
  {NOTE_REST,50},
  {NOTE_A5, 100},
  {NOTE_REST,50},
  {NOTE_E5, 100},
  {NOTE_REST,50},
  {NOTE_E5, 100},
  {NOTE_REST,50},
  {NOTE_G4, 100},
  {NOTE_REST,50},
  {NOTE_G4, 100},
  {NOTE_REST,50},
  {NOTE_D5, 100},
  {NOTE_REST,50},
  {NOTE_D5, 100},
  {NOTE_REST,50},
  {NOTE_G5, 100},
  {NOTE_REST,50},
  {NOTE_G5, 100},
  {NOTE_REST,50},
  {NOTE_D5, 100},
  {NOTE_REST,50},
  {NOTE_D5, 100},
  {NOTE_REST,50},
  {NOTE_D4, 100},
  {NOTE_REST,50},
  {NOTE_D4, 100},
  {NOTE_REST,50},
  {NOTE_A4, 100},
  {NOTE_REST,50},
  {NOTE_A4, 100},
  {NOTE_REST,50},
  {NOTE_D5, 100},
  {NOTE_REST,50},
  {NOTE_D5, 100},
  {NOTE_REST,50},
  {NOTE_E5, 100},
  {NOTE_REST,50},
  {NOTE_E5, 100}, 
  {NOTE_REST,50},
  {NOTE_F5, 100},
  {NOTE_REST,50},
  {NOTE_F5, 100},
  {NOTE_REST,50},
  {NOTE_E5, 100},
  {NOTE_REST,50},
  {NOTE_E5, 100},
  {NOTE_REST,50},
  {NOTE_D5, 100},
  {NOTE_REST,50},
  {NOTE_D5, 100},
  {NOTE_REST,50},
  {NOTE_E5, 100},
  {NOTE_REST,50},
  {NOTE_E5, 100},
  {NOTE_REST,50}
};

note_type just_beep[] =
{
  {NOTE_E5,   700},
  {NOTE_REST, 300}
};

song_type groove_song={groove, sizeof(groove)/sizeof(note_type)};
song_type temple_song={temple, sizeof(temple)/sizeof(note_type)};
song_type scale_song={scale, 8};
song_type just_beep_song={just_beep, sizeof(just_beep)/sizeof(note_type)};
song_type alarm_song;

song_type all_songs[]=
{
  groove_song,
  temple_song,
  scale_song,
  just_beep_song
};

int num_songs = sizeof(all_songs)/sizeof(song_type);

song_type song = temple_song;

uint32_t song_button_press_ms = 0;
int      last_song_button_state = 1;
#define SONG_BUTTON_PRESS_TIME 500

void start_song( void )
{
  note_type first_note;

  Serial.println("Start Song!!!");
  song_playing = true;

  song_start_ms = millis();

  // play the first note
  first_note = song.notes[0];
  if (first_note.freq == NOTE_REST)
  {
    noTone(SPEAKER_PIN);
  }
  else
  {
    tone(SPEAKER_PIN,first_note.freq);
  }
  
  // mark when the next note should start.
  song_next_note_ms = song_start_ms + first_note.duration_ms;

  // check for special case:  one note song.
  if (song.num_notes == 1) song_current_note_index = 0;
  else song_current_note_index = 1;
}

void stop_song( void )
{
  noTone(SPEAKER_PIN);
  song_playing = false;
}

void play_song( void )
{
  uint32_t current_time;
  note_type  note;
  
  current_time = millis();
 
  // is it time to play the next note?
  if (current_time > song_next_note_ms)
  {
    note = song.notes[song_current_note_index];
    if (note.freq == NOTE_REST)
    {
      noTone(SPEAKER_PIN);
    }
    else
    {
      tone(SPEAKER_PIN,note.freq);
    }
    
    // set up for the next note
    song_next_note_ms = current_time + note.duration_ms;
    song_current_note_index++;
    if (song_current_note_index == song.num_notes)
    {
      song_current_note_index = 0;
    } 
  }
}

void check_for_song_toggle(void)
{
  int current_song_button_state;
  int current_ms;

  current_ms = millis();
  
  current_song_button_state = digitalRead(SONG_BUTTON_PIN);
  if (current_song_button_state == 0)
  { 
    /* if the button was just pressed, mark the time when it was pressed */
    if (last_song_button_state == 1)
    {
      Serial.print("Song button pressed.  Time: ");
      Serial.println(current_ms);
      song_button_press_ms = current_ms;
    }
  }
  else
  {
    if (last_song_button_state == 0)
    {
      /* if the button was released, see if it was down long enough to warrant 
       *  a toggle
       */
      Serial.println("button release");
      
      if (current_ms > song_button_press_ms + SONG_BUTTON_PRESS_TIME)
      {
        Serial.println("toggle");
        
        /* toggle the song */
        if (song_playing)
        {
          stop_song();
        }
        else
        {
          start_song();
        }
      }
    }
  }
  
  last_song_button_state = current_song_button_state;
}

void setup (void)
{
  pinMode(SPEAKER_PIN, OUTPUT);
  pinMode(SONG_BUTTON_PIN, INPUT_PULLUP);

  Serial.begin(9600);

  Serial.println("initialized");
}

void loop (void)
{

  check_for_song_toggle();
  if (song_playing)
  {
    play_song();
  }

}
