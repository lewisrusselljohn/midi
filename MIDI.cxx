#include <Windows.h>
#include <mmeapi.h>
#include <mmsystem.h>
#include <stdio.h>

HMIDIOUT hmo = NULL;
HMIDIIN hmi = NULL;

/*
    Forward declarations.
*/
void InitMidiIn();
void InitMidiOut();
void PrintStandardTable();
void SelectInstrument(int);
void PrintHelp();
void CALLBACK MidiInCallback(HMIDIIN hMidiIn, UINT wMsg, 
    DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2);

/*
    wMsg: type of MIDI message
    dwInstance: user defined value specified when opening the input device
    dwParam1, dwParam2: information associated with the MIDI message. if wMsg is 
    MIM_DATA, dwParam1 contains the MIDI message.
*/

int main()
{
    InitMidiIn();
    InitMidiOut();

    if(hmi)
    {
        PrintHelp();
        midiInStart(hmi);

        bool run = true;
        while (run)
        {
            char input[100];

            printf("command: ");
            fgets(input, sizeof(input), stdin);

            if (strstr(input, "list") != NULL)
            {
                PrintStandardTable();
            }

            if (strstr(input, "select") != NULL)
            {
                printf("Input instrument number: ");
                fgets(input, sizeof(input), stdin);
                int left, middle, right;
                left = (int)input[0] - 48; middle = (int)input[1] - 48; right = (int)input[2]-48;

                int selection = 0;
                if (left >= 0 && left <= 9) selection += left * 100;
                
                if (middle >= 0 && middle <= 9) selection += middle * 10;

                if (right >= 0 && right <= 9) selection += right;
                
                SelectInstrument(selection);
            }

            if (strstr(input, "help") != NULL)
            {

            }

            if (strstr(input, "exit") != NULL)
            {
                run = false;
            }
        }
    }

    if (hmo != NULL)
    {
        midiOutClose(hmo);
    }
    if (hmi != NULL)
    {
        midiInStop(hmi);
        midiInClose(hmi);
    }
    return 0;
}


void InitMidiIn()
{
    if (hmi == NULL)
    {
        MMRESULT result = midiInOpen(&hmi, 0, reinterpret_cast<DWORD_PTR>(&MidiInCallback), 0, CALLBACK_FUNCTION);
        if (result == MMSYSERR_NOERROR)
            printf("Successfully opened MIDI in device.\n");
        else
            printf("Unable to open MIDI in device - make sure the MIDI Keyboard is plugged in and turned on.\n");
    }
}

void InitMidiOut()
{
    if (hmo == NULL)
    {
        MMRESULT result = midiOutOpen(&hmo, MIDI_MAPPER, 0, 0, CALLBACK_NULL);
        if (result == MMSYSERR_NOERROR)
            printf("Successfully opened MIDI out device.\n");
        else
            printf("Unable to open MIDI out device.\n");
    }
}

void PrintStandardTable()
{
    const char* instruments[16] =
    {
        "piano", "chromatic percussion", "organ", "guitar",
        "bass", "strings", "ensemble", "brass",
        "reed", "pipe", "synth lead", "synth pad",
        "synth effects", "ethnic", "percussive", "sound effects"
    };

    for (int i = 0; i < 16; i++)
    {
        printf(" %3d -%3d      %s\n", i * 8, i * 8 + 7, instruments[i]);
    }
}

void SelectInstrument(int selection)
{
    if (selection >= 0 && selection <= 127)
    {
        // Prepare the MIDI program change message for program number 1 (grand piano)
        BYTE status = 0xC0;    // MIDI program change status (channel 0)
        BYTE program = selection;      // Program number (1 for grand piano)
        DWORD midiMessage = status | (program << 8);

        // Send the MIDI program change message to select the grand piano sound
        if (hmo != NULL)
        {
            midiOutShortMsg(hmo, midiMessage);
            printf("\n instrument selected.\n");
        }
    }
    else
    {
        printf("Invalid selection. Input must be in format xxx, where \n");
        printf("each x is between 0 and 9 inclusive.\n");
    }
}

void PrintHelp()
{
    printf("Type a command and enter to operate this program.\n");
    printf(" commands: \n");
    printf(" list        - prints a list of instruments available to play.\n");
    printf(" select x    - selects the instrument x which must match a value from list.\n");
    printf(" exit        - quit the program\n");
}

void CALLBACK MidiInCallback(HMIDIIN hMidiIn, UINT wMsg, 
    DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2)
{
    // Handle MIDI messages from the MIDI keyboard

    switch (wMsg)
    {
    case MIM_DATA:
    {
        DWORD midi_msg = static_cast<DWORD>(dwParam1);
        // Extract the MIDI message components

        /*
            status == 144 for down, 128 for up
            data1 is the pitch (which note was pressed)
            data2 is the velocity (how hard it was pressed)
        */
        BYTE status = static_cast<BYTE>((midi_msg >> 0) & 0xFF);
        BYTE data1 = static_cast<BYTE>((midi_msg >> 8) & 0xFF);
        BYTE data2 = static_cast<BYTE>((midi_msg >> 16) & 0xFF);

        if (hmo == NULL)
        {
            MMRESULT result = midiOutOpen(&hmo, MIDI_MAPPER, 0, 0, CALLBACK_NULL);
            if (result == MMSYSERR_NOERROR)
                printf("Successful open\n");
            else
                printf("Possible error.\n");
        }

        /*
            Noticeable delay - might be issue with underlying API used my midiOut.
            Try with WASAPI to test.
        */

        // Send the message to the MIDI output device
        midiOutShortMsg(hmo, midi_msg);

        break;
    }
    }
}
