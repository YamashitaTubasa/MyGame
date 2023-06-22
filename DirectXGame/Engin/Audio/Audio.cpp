#include "Audio.h"

Audio* Audio::GetInstance()
{
	static Audio instance;

	return &instance;
}

void Audio::Initialize(const std::string& directoryPath)
{	
	directoryPath_ = directoryPath;

	// XAudioエンジンのインスタンスを生成
	result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	assert(SUCCEEDED(result));

	// マスターボイスを生成
	result = xAudio2->CreateMasteringVoice(&mVoice);
	assert(SUCCEEDED(result));
}

void Audio::Finalize()
{
	// XAudio2解放
	xAudio2.Reset();

	// 音声データ解放
	std::map<std::string, SoundData>::iterator it = soundDatas_.begin();
	for (; it != soundDatas_.end(); ++it) {
		Unload(&it->second);
	}
	soundDatas_.clear();
}

void Audio::LoadWave(const std::string& filename)
{
	if (soundDatas_.find(filename) != soundDatas_.end()) {
		// 重複読み込みなので、何もせず抜ける
		return;
	}

	// ディレクトリパスとファイル名を連結してフルパスを得る
	std::string fullpath = directoryPath_ + filename;

	// ①ファイルオープン
	// ファイル入力ストリームのインスタンス
	std::ifstream file;
	// .wavファイルバイナリモードで開く
	file.open(fullpath, std::ios_base::binary);
	// ファイルオープン失敗を検出
	assert(file.is_open());

	// ②.WAVデータ読み込み
	// RIFFヘッダーの読み込み
	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));
	// ファイルがRIFFがチェック
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0) {
		assert(0);
	}
	// タイプがWAVEかチェック
	if (strncmp(riff.type, "WAVE", 4) != 0) {
		assert(0);
	}
	// Formatチャンクの読み込み
	FormatChunk format = {};
	// チャンクヘッダーの確認
	file.read((char*)&format, sizeof(ChunkHeader));
	if (strncmp(format.chunk.id, "fmt ", 4) != 0) {
		assert(0);
	}

	// チャンク本体の読み込み
	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char*)&format.fmt, format.chunk.size);

	// Dataチャンクの読み込み
	ChunkHeader data;;
	file.read((char*)&data, sizeof(data));
	// JUNKチャンクを検出した場合
	if (strncmp(data.id, "JUNK", 4) == 0) {
		// 読み取り位置をJUNKチャンクの終わりまで進める
		file.seekg(data.size, std::ios_base::cur);
		// 再読み込み
		file.read((char*)&data, sizeof(data));
	}
	if (strncmp(data.id, "data", 4) != 0) {
		assert(0);
	}

	// DATAチャンクのデータ部(波形データ)の読み込み
	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);

	// WAVEファイルと閉じる
	file.close();

	// ④読み込んだ音声データをreturn
	// returnする為の音声データ
	SoundData soundData = {};

	soundData.wfex = format.fmt;
	soundData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	soundData.bufferSize = data.size;

	// サウンドデータを連想配列に格納
	soundDatas_.insert(std::make_pair(filename, soundData));
}

void Audio::Unload(SoundData* soundData) {
	// バッファのメモリを解放
	delete[] soundData->pBuffer;

	soundData->pBuffer = 0;
	soundData->bufferSize = 0;
	soundData->wfex = {};
}

void Audio::PlayWave(const std::string& filename) {
	std::map<std::string, SoundData>::iterator it = soundDatas_.find(filename);
	// 未読み込みの検出
	assert(it != soundDatas_.end());

	// サウンドデータの参照を取得
	SoundData& soundData = it->second;;

	// 波形フォーマットを元にSourceVoiceの生成
	IXAudio2SourceVoice* pSourceVoice = nullptr;
	result = xAudio2->CreateSourceVoice(&pSourceVoice, &soundData.wfex);
	assert(SUCCEEDED(result));

	// 再生する波形データの設定
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData.pBuffer;
	buf.AudioBytes = soundData.bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;

	// 波形データの再生
	result = pSourceVoice->SubmitSourceBuffer(&buf);
	result = pSourceVoice->Start();
}

void Audio::LoopPlayWave(const std::string& filename)
{
	std::map<std::string, SoundData>::iterator it = soundDatas_.find(filename);
	// 未読み込みの検出
	assert(it != soundDatas_.end());
	// サウンドデータの参照を取得
	SoundData& soundData = it->second;

	// 波形フォーマットをもとにSoundVoiceの生成
	result = xAudio2->CreateSourceVoice(&sVoice, &soundData.wfex);
	assert((SUCCEEDED(result)));

	// 再生する波形データの設定
	buf.pAudioData = soundData.pBuffer;
	buf.AudioBytes = soundData.bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;

	// 音声ループ
	buf.LoopCount = XAUDIO2_LOOP_INFINITE;

	// 波形データの再生
	result = sVoice->SubmitSourceBuffer(&buf);
	result = sVoice->Start();
}

void Audio::StopWave(const std::string& filename)
{
	std::map<std::string, SoundData>::iterator it = soundDatas_.find(filename);
	// 未読み込みの検出
	assert(it != soundDatas_.end());
	// サウンドデータの参照を取得
	SoundData& soundData = it->second;

	// 波形フォーマットをもとにSoundVoiceの生成
	result = xAudio2->CreateSourceVoice(&sVoice, &soundData.wfex);
	assert((SUCCEEDED(result)));

	// 再生する波形データの設定
	buf.pAudioData = soundData.pBuffer;
	buf.AudioBytes = soundData.bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;

	// 波形データの再生
	result = sVoice->SubmitSourceBuffer(&buf);
	result = sVoice->Stop();
	result = sVoice->FlushSourceBuffers();
	sVoice->DestroyVoice();
}

void Audio::SetVolume(const std::string& filename, float volume)
{
	std::map<std::string, SoundData>::iterator it = soundDatas_.find(filename);
	// 未読み込みの検出
	assert(it != soundDatas_.end());
	// サウンドデータの参照を取得
	SoundData& soundData = it->second;

	// 波形フォーマットをもとにSoundVoiceの生成
	result = xAudio2->CreateSourceVoice(&sVoice, &soundData.wfex);
	assert((SUCCEEDED(result)));

	// 再生する波形データの設定
	buf.pAudioData = soundData.pBuffer;
	buf.AudioBytes = soundData.bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;

	// 波形データの再生
	result = sVoice->SubmitSourceBuffer(&buf);
	result = sVoice->SetVolume(volume);
}

