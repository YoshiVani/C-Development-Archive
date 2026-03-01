#include <windows.h>
#include <stdio.h>
#include <stdbool.h>
#include <mmsystem.h>

// winmm.libをリンクする必要があります（gccの場合は-lwinmmフラグ）

// Windows APIのINPUT構造体を使用して、キーボードの物理的な動作をシミュレートする関数
// isKeyDownがtrueならキーを押し、falseならキーを離す
void sendKey(int key, bool isKeyDown) {
    if (key == 0) return;

    INPUT input = {0};
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = key;　// 仮想キーコードを設定
    
    // キーを離す動作の時は、フラグにKEYEVENTF_KEYUPを指定
    if (!isKeyDown) {
        input.ki.dwFlags = KEYEVENTF_KEYUP;
    }

   // OSの入力キューにキーイベントを送信
    SendInput(1, &input, sizeof(INPUT));
}

// MIDIノート番号を仮想キーコードに変換する関数
// オクターブごとに同じキー配置が繰り返されるように設定
int mapMidiToScanCode(int midiNote) {
	switch(midiNote){
		// 低音域から高音域まで、特定のキー（Z, S, X, D... や Q, 2, W, 3...）にマッピング
        // 12〜59: 下位オクターブのループ設定
		case 12: return 0xbc;
		case 13: return 0x4c;
		case 14: return 0xbe;
		case 15: return 0xba;
		case 16: return 0xbf;
		case 17: return 0x49;
		case 18: return 0x39;
		case 19: return 0x4f;
		case 20: return 0x30;
		case 21: return 0x50;
		case 22: return 0xbd;
		case 23: return 0xdb;
		
		case 24: return 0xbc;
		case 25: return 0x4c;
		case 26: return 0xbe;
		case 27: return 0xba;
		case 28: return 0xbf;
		case 29: return 0x49;
		case 30: return 0x39;
		case 31: return 0x4f;
		case 32: return 0x30;
		case 33: return 0x50;
		case 34: return 0xbd;
		case 35: return 0xdb;
		
		case 36: return 0xbc;
		case 37: return 0x4c;
		case 38: return 0xbe;
		case 39: return 0xba;
		case 40: return 0xbf;
		case 41: return 0x49;
		case 42: return 0x39;
		case 43: return 0x4f;
		case 44: return 0x30;
		case 45: return 0x50;
		case 46: return 0xbd;
		case 47: return 0xdb;
		
		case 48: return 0xbc;
		case 49: return 0x4c;
		case 50: return 0xbe;
		case 51: return 0xba;
		case 52: return 0xbf;
		case 53: return 0x49;
		case 54: return 0x39;
		case 55: return 0x4f;
		case 56: return 0x30;
		case 57: return 0x50;
		case 58: return 0xbd;
		case 59: return 0xdb;
		
		case 60: return 0x5a;
		case 61: return 0x53;
		case 62: return 0x58;
		case 63: return 0x44;
		case 64: return 0x43;
		case 65: return 0x56;
		case 66: return 0x47;
		case 67: return 0x42;
		case 68: return 0x48;
		case 69: return 0x4e;
		case 70: return 0x4a;
		case 71: return 0x4d;
		
		case 72: return 0x51;
		case 73: return 0x32;
		case 74: return 0x57;
		case 75: return 0x33;
		case 76: return 0x45;
		case 77: return 0x52;
		case 78: return 0x35;
		case 79: return 0x54;
		case 80: return 0x36;
		case 81: return 0x59;
		case 82: return 0x37;
		case 83: return 0x55;
		
		case 84: return 0x51;
		case 85: return 0x32;
		case 86: return 0x57;
		case 87: return 0x33;
		case 88: return 0x45;
		case 89: return 0x52;
		case 90: return 0x35;
		case 91: return 0x54;
		case 92: return 0x36;
		case 93: return 0x59;
		case 94: return 0x37;
		case 95: return 0x55;
		
		case 96: return 0x51;
		case 97: return 0x32;
		case 98: return 0x57;
		case 99: return 0x33;
		case 100: return 0x45;
		case 101: return 0x52;
		case 102: return 0x35;
		case 103: return 0x54;
		case 104: return 0x36;
		case 105: return 0x59;
		case 106: return 0x37;
		case 107: return 0x55;
		
		default: return 0;
	}
}

// MIDIイベントのコールバック関数
void CALLBACK MidiInProc(HMIDIIN hMidiIn, UINT wMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2) {
    if (wMsg == MIM_DATA) {
		// dwParam1の中にMIDIメッセージを格納（下位バイトからStatus,Data1,Data2）
        BYTE status = (BYTE)(dwParam1 & 0xFF);
        BYTE data1 = (BYTE)((dwParam1 >> 8) & 0xFF); // MIDIノート番号
        1BYTE data2 = (BYTE)((dwParam1 >> 16) & 0xFF); // ベロシティ(叩いた強さ)
		
        
		// ノート番号をキーボードのキーコードに変換
        int key = mapMidiToScanCode(data1); 

		// ノートオン判定（鍵盤が押された、またはベロシティが0より大きい場合）
        if ((status & 0xF0) == 0x90 && data2 > 0) {
            sendKey(key, true);
        }
        // ノートオフ判定（鍵盤が離された、またはノートオンでベロシティが0の場合）
        else if ((status & 0xF0) == 0x80 || ((status & 0xF0) == 0x90 && data2 == 0)) {
            sendKey(key, false);
        }
    }
}

int main(){
	HMIDIIN hMidiIn;
	UINT deviceId = 0;　// 0番目のMIDIデバイスを使用
	
	// MIDI入力デバイスを開く
	if (midiInOpen(&hMidiIn, deviceId, (DWORD_PTR)MidiInProc, 0, CALLBACK_FUNCTION) != MMSYSERR_NOERROR) {
        printf("\nThis programming is for testing. It shows which key you press.\n");
        printf("MIDI device couldn't be opened.\n");
        return 1;
    }
	printf("MIDI device opened successfully.\n");
	
	// MIDI入力監視を開始
    midiInStart(hMidiIn);
    printf("Waiting for MIDI events...\nPress Ctrl+C to exit.\n");

	// プログラムが終了しないように待機
    while (true) {
        Sleep(100);
    }
	
	// 終了処理（実際は無限ループのため、Ctrl+C等による強制終了を想定）
	midiInStop(hMidiIn);
	midiInClose(hMidiIn);
    
    return 0;
}