/**
 * @file Audio.h
 * @brief サウンドに関するクラス
 * @author Yamashita Tubasa
 */

#pragma once

#pragma warning(push)
#pragma warning(disable:4265)
#pragma warning(disable:4365)
#pragma warning(disable:4625)
#pragma warning(disable:4626)
#pragma warning(disable:4668)
#pragma warning(disable:4820)
#pragma warning(disable:5039)
#pragma warning(disable:5204)
#pragma warning(disable:5220)
#include <xaudio2.h>
#include <fstream>
#include <wrl.h>
#include <DirectXMath.h>
#include <map>
#include <string>
#pragma warning(pop)

#pragma comment(lib, "xaudio2.lib")

namespace MyEngine
{
	//定数バッファ用データ構造体(マテリアル)
	struct ConstBufferDataMaterial {
		DirectX::XMFLOAT4 color_;//色(RGBA)
	};

	//定数バッファ用データ構造体(3D変換行列)
	struct ConstBufferDataTransform {
		DirectX::XMMATRIX mat_; //3D変換行列
	};

	// チャンクヘッダ
	struct ChunkHeader
	{
		char id_[4]; // チャンク前のID
		int32_t size_; // チャンクサイズ
	};

	// RIFFヘッダチャンク
	struct RiffHeader
	{
		ChunkHeader chunk_; // "RIFF"
		char type_[4]; // "WAVE"
	};

	// FMTチャンク
	struct FormatChunk {
		ChunkHeader chunk_; // "fmt"
		WAVEFORMATEX fmt_; // 波形フォーマット
	};

	// 音声データ
	struct SoundData {
		// 波形フォーマット
		WAVEFORMATEX wfex_;
		// バッファの先頭アドレス
		BYTE* pBuffer_;
		// バッファのサイズ
		unsigned int bufferSize_;
	};

	class Audio final
	{
	public:
		template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

		using XMFLOAT3 = DirectX::XMFLOAT3;
		using XMFLOAT4 = DirectX::XMFLOAT4;
		using XMMATRIX = DirectX::XMMATRIX;

	private:
		Audio() = default;
		~Audio() = default;

	public:
		Audio(const Audio& obj) = delete;
		Audio& operator=(const Audio& obj) = delete;

		// インスタンスを生成
		static Audio* GetInstance();

		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="directoryPath"></param>
		void Initialize(const std::string& directoryPath = "Resources/BGM/");

		/// <summary>
		/// 解放
		/// </summary>
		void Finalize();

		/// <summary>
		/// 音声データの読み込み
		/// </summary>
		/// <param name="filename">WAVファイル名</param>
		void LoadWave(const std::string& filename);

		/// <summary>
		/// 音声データ解放
		/// </summary>
		/// <param name="soundData">サウンドデータ</param>
		void Unload(SoundData* soundData);

		/// <summary>
		/// 音声再生
		/// </summary>
		/// <param name="filename">WAVファイル名</param>
		void PlayWave(const std::string& filename);

		/// <summary>
		/// 音声のループ再生
		/// </summary>
		/// <param name="filename">WAVファイル名</param>
		void LoopPlayWave(const std::string& filename);

		/// <summary>
		/// 音声停止
		/// </summary>
		/// <param name="filename">WAVファイル名</param>
		void StopWave(const std::string& filename);

		/// <summary>
		/// 音量調節
		/// </summary>
		/// <param name="filename">WAVファイル名</param>
		/// <param name="volume">音量</param>
		void SetVolume(const std::string& filename, float volume);

	public:
		/// <summary>
		/// Audio2の取得
		/// </summary>
		/// <returns></returns>
		[[maybe_unused]] IXAudio2* GetxAudio2() { return xAudio2_.Get(); }

		/// <summary>
		/// 音量の取得
		/// </summary>
		/// <returns></returns>
		[[maybe_unused]] float GetVolume()const { return mVolume_; }

	private:
		HRESULT result_;
		ComPtr<IXAudio2> xAudio2_; // XAudio2インターフェイス
		IXAudio2MasteringVoice* mVoice_; // マスターボイス
		IXAudio2SourceVoice* sVoice_; // ソースボイス
		XAUDIO2_BUFFER buf_{};

		// サウンドデータの連想配列
		std::map<std::string, SoundData> soundDatas_;
		// サウンド格納ディレクトリ
		std::string directoryPath_;

		float mVolume_;
	};
};