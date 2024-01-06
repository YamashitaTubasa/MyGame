/**
 * @file Audio.cpp
 * @brief サウンドに関するクラス
 * @author Yamashita Tubasa
 */

#include "Audio.h"

MyEngine::Audio* MyEngine::Audio::GetInstance()
{
	static Audio instance;

	return &instance;
}

void MyEngine::Audio::Initialize(const std::string& directoryPath)
{	
	directoryPath_ = directoryPath;

	// XAudioエンジンのインスタンスを生成
	result_ = XAudio2Create(&xAudio2_, 0, XAUDIO2_DEFAULT_PROCESSOR);
	assert(SUCCEEDED(result_));

	// マスターボイスを生成
	result_ = xAudio2_->CreateMasteringVoice(&mVoice_);
	assert(SUCCEEDED(result_));
}

void MyEngine::Audio::Finalize()
{
	// XAudio2解放
	xAudio2_.Reset();

	// 音声データ解放
	std::map<std::string, SoundData>::iterator it = soundDatas_.begin();
	for (; it != soundDatas_.end(); ++it) {
		Unload(&it->second);
	}
	soundDatas_.clear();
}

void MyEngine::Audio::LoadWave(const std::string& filename)
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
	if (strncmp(riff.chunk_.id_, "RIFF", 4) != 0) {
		assert(0);
	}
	// タイプがWAVEかチェック
	if (strncmp(riff.type_, "WAVE", 4) != 0) {
		assert(0);
	}
	// Formatチャンクの読み込み
	FormatChunk format = {};
	// チャンクヘッダーの確認
	file.read((char*)&format, sizeof(ChunkHeader));
	if (strncmp(format.chunk_.id_, "fmt ", 4) != 0) {
		assert(0);
	}

	// チャンク本体の読み込み
	assert(format.chunk_.size_ <= sizeof(format.fmt_));
	file.read((char*)&format.fmt_, format.chunk_.size_);

	// Dataチャンクの読み込み
	ChunkHeader data;;
	file.read((char*)&data, sizeof(data));
	// JUNKチャンクを検出した場合
	if (strncmp(data.id_, "JUNK", 4) == 0) {
		// 読み取り位置をJUNKチャンクの終わりまで進める
		file.seekg(data.size_, std::ios_base::cur);
		// 再読み込み
		file.read((char*)&data, sizeof(data));
	}
	if (strncmp(data.id_, "data", 4) != 0) {
		assert(0);
	}

	// DATAチャンクのデータ部(波形データ)の読み込み
	char* pBuffer = new char[(size_t)data.size_];
	file.read(pBuffer, data.size_);

	// WAVEファイルと閉じる
	file.close();

	// ④読み込んだ音声データをreturn
	// returnする為の音声データ
	SoundData soundData = {};

	soundData.wfex_ = format.fmt_;
	soundData.pBuffer_ = reinterpret_cast<BYTE*>(pBuffer);
	soundData.bufferSize_ = (unsigned int)data.size_;

	// サウンドデータを連想配列に格納
	soundDatas_.insert(std::make_pair(filename, soundData));
}

void MyEngine::Audio::Unload(SoundData* soundData) {
	// バッファのメモリを解放
	delete[] soundData->pBuffer_;

	soundData->pBuffer_ = 0;
	soundData->bufferSize_ = 0;
	soundData->wfex_ = {};
}

void MyEngine::Audio::PlayWave(const std::string& filename) {
	std::map<std::string, SoundData>::iterator it = soundDatas_.find(filename);
	// 未読み込みの検出
	assert(it != soundDatas_.end());

	// サウンドデータの参照を取得
	SoundData& soundData = it->second;;

	// 波形フォーマットを元にSourceVoiceの生成
	IXAudio2SourceVoice* pSourceVoice = nullptr;
	result_ = xAudio2_->CreateSourceVoice(&pSourceVoice, &soundData.wfex_);
	assert(SUCCEEDED(result_));

	// 再生する波形データの設定
	buf_.pAudioData = soundData.pBuffer_;
	buf_.AudioBytes = soundData.bufferSize_;
	buf_.Flags = XAUDIO2_END_OF_STREAM;

	// 波形データの再生
	result_ = pSourceVoice->SubmitSourceBuffer(&buf_);
	result_ = pSourceVoice->Start();
}

void MyEngine::Audio::LoopPlayWave(const std::string& filename)
{
	std::map<std::string, SoundData>::iterator it = soundDatas_.find(filename);
	// 未読み込みの検出
	assert(it != soundDatas_.end());
	// サウンドデータの参照を取得
	SoundData& soundData = it->second;

	// 波形フォーマットをもとにSoundVoiceの生成
	result_ = xAudio2_->CreateSourceVoice(&sVoice_, &soundData.wfex_);
	assert((SUCCEEDED(result_)));

	// 再生する波形データの設定
	buf_.pAudioData = soundData.pBuffer_;
	buf_.AudioBytes = soundData.bufferSize_;
	buf_.Flags = XAUDIO2_END_OF_STREAM;

	// 音声ループ
	buf_.LoopCount = XAUDIO2_LOOP_INFINITE;

	// 波形データの再生
	result_ = sVoice_->SubmitSourceBuffer(&buf_);
	result_ = sVoice_->Start();
}

void MyEngine::Audio::StopWave(const std::string& filename)
{
	std::map<std::string, SoundData>::iterator it = soundDatas_.find(filename);
	// 未読み込みの検出
	assert(it != soundDatas_.end());
	// サウンドデータの参照を取得
	SoundData& soundData = it->second;

	// 波形フォーマットをもとにSoundVoiceの生成
	result_ = xAudio2_->CreateSourceVoice(&sVoice_, &soundData.wfex_);
	assert((SUCCEEDED(result_)));

	// 再生する波形データの設定
	buf_.pAudioData = soundData.pBuffer_;
	buf_.AudioBytes = soundData.bufferSize_;
	buf_.Flags = XAUDIO2_END_OF_STREAM;

	// 波形データの再生
	result_ = sVoice_->SubmitSourceBuffer(&buf_);
	result_ = sVoice_->Stop();
	result_ = sVoice_->FlushSourceBuffers();
	sVoice_->DestroyVoice();
}

void MyEngine::Audio::SetVolume(const std::string& filename, float volume)
{
	std::map<std::string, SoundData>::iterator it = soundDatas_.find(filename);
	// 未読み込みの検出
	assert(it != soundDatas_.end());
	// サウンドデータの参照を取得
	SoundData& soundData = it->second;

	// 波形フォーマットをもとにSoundVoiceの生成
	result_ = xAudio2_->CreateSourceVoice(&sVoice_, &soundData.wfex_);
	assert((SUCCEEDED(result_)));

	// 再生する波形データの設定
	buf_.pAudioData = soundData.pBuffer_;
	buf_.AudioBytes = soundData.bufferSize_;
	buf_.Flags = XAUDIO2_END_OF_STREAM;

	// 波形データの再生
	result_ = sVoice_->SubmitSourceBuffer(&buf_);
	result_ = sVoice_->SetVolume(volume);
}

