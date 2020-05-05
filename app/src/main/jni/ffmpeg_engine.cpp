//
// Created by zcj on 2020/5/5.
//

#include <unistd.h>
#include "ffmpeg_engine.h"
#include "log_util.h"

AVFormatContext *pFormatCtx;
AVCodecContext *pCodecCtx;
AVCodec *pCodex;
AVPacket *packet;
AVFrame *frame;
SwrContext *swrContext;
uint8_t *out_buffer;
int out_channel_nb;
int audio_stream_idx = -1;

//FFMPEG4.0 音频解码解封装 https://blog.51cto.com/4095821/2402550

int createFFmpeg(unsigned *rate, unsigned *channel)
{
    av_register_all();
    //char *input = "http://audio04.dmhmusic.com/71_53_T10046885865_128_4_1_0_sdk-cpm/cn/0206/M00/7B/02/ChR461urPneAN8rbADnHLEU29-Y184.mp3?xcode=4eda71c9bc5e3dcc649133c195093eb068cb3ee";
    char *input = "/storage/emulated/0/Android/data/com.zcj.teana/files/test.mp3";
    pFormatCtx = avformat_alloc_context();
    int error;
    char buff[] = "";
    //打开地址并获取里面的类容
    //avformat_network_init();
    if ((error = avformat_open_input(&pFormatCtx, input, NULL, NULL)) < 0)
    {
        av_strerror(error, buff, 1024);
        LOGE("couldn't open url %s: %d(%s)\n", input, error, buff);
        LOGE("打开流媒体失败");
        return -1;
    }
    //获取流媒体信息
    if (avformat_find_stream_info(pFormatCtx, NULL) < 0)
    {
        LOGE("获取流媒体信息失败");
        return -2;
    }

    audio_stream_idx = av_find_best_stream(pFormatCtx, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
    if (audio_stream_idx < 0)
    {
        LOGE("not find audio stream");
        return -5;
    }
    else
    {
        LOGI("audio stream index = %d", audio_stream_idx);
    }

    //查找解码器
    AVCodecParameters *pCodecParam = pFormatCtx->streams[audio_stream_idx]->codecpar;
    LOGI("codec_id = %d", pCodecParam->codec_id);
    pCodex = avcodec_find_decoder(pCodecParam->codec_id);
    if (!pCodex)
    {
        LOGE("unsupport codec.");
        return -3;
    }

    pCodecCtx = avcodec_alloc_context3(pCodex);

    //把stream信息中的参数拷贝到解码Context中
    avcodec_parameters_to_context(pCodecCtx, pCodecParam);

    if (avcodec_open2(pCodecCtx, pCodex, NULL) < 0)
    {
        LOGE("could not open codec.");
        return -4;
    }
    else
    {
        LOGI("codec bitRate = %d, sampleRate = %d, channels = %d, fmt = %d",
             pCodecCtx->bit_rate, pCodecCtx->sample_rate, pCodecCtx->channels,
             pCodecCtx->sample_fmt);
    }
    packet = (AVPacket *) av_malloc(sizeof(AVPacket));
    frame = av_frame_alloc();

    //转换成pcm
    swrContext = swr_alloc();

    //44100 * 2
    out_buffer = (uint8_t *) av_malloc(44100 * 2);
    uint64_t out_ch_layout = AV_CH_LAYOUT_STEREO;
    //输出采样位数 16位
    //AVSampleFormat out_format = AV_SAMPLE_FMT_S16;
    auto out_format = AV_SAMPLE_FMT_S16;
    int out_sample_rate = pCodecCtx->sample_rate;

    swr_alloc_set_opts(swrContext, out_ch_layout, out_format, out_sample_rate,
                       pCodecCtx->channel_layout, pCodecCtx->sample_fmt, pCodecCtx->sample_rate, 0,
                       NULL);
    swr_init(swrContext);
    //获取通道数 2
    out_channel_nb = av_get_channel_layout_nb_channels(AV_CH_LAYOUT_STEREO);
    *rate = pCodecCtx->sample_rate;
    *channel = pCodecCtx->channels;
    return 0;
}

int getPCM(void **pcm, size_t *pcm_size)
{
    LOGI("getPCM..., audio_stream_idx = %d, pCodecCtx = %p, frame = %p, packet = %p, swrContext = %p, out_buffer = %p, out_channel_nb = %d\n",
         audio_stream_idx, pCodecCtx, frame, packet, swrContext, out_buffer, out_channel_nb);
    int got_frame;
    int re;
    while (av_read_frame(pFormatCtx, packet) >= 0)
    {
        LOGI("av_read_frame...");
        LOGI("packet, pts = %d, dts = %d, size = %d, duration = %d, pos = %d\n",
             packet->pts, packet->dts, packet->size, packet->duration, packet->pos);
        if (packet->stream_index == audio_stream_idx)
        {
            //解码
            re = avcodec_send_packet(pCodecCtx, packet);
            if (re < 0)
            {
                LOGE("send packet failed, re = %d\n", re);
                return 0;
            }
            while (re >= 0)
            {
                re = avcodec_receive_frame(pCodecCtx, frame);
                if (re == AVERROR(EAGAIN) || re == AVERROR_EOF)
                {
                    return 0;
                }
                else if (re < 0)
                {
                    LOGE("error during decoding\n");
                    return 0;
                }

                LOGI("frame sample_rate = %d, channels = %d, size = %d, duration = %d",
                     frame->sample_rate, frame->channels, frame->pkt_size, frame->pkt_duration);

                swr_convert(swrContext, &out_buffer, 44100 * 2, (const uint8_t **) frame->data,
                            frame->nb_samples);
                //缓冲区大小
                int size = av_samples_get_buffer_size(NULL, out_channel_nb, frame->nb_samples,
                                                      AV_SAMPLE_FMT_S16, 1);
                *pcm = out_buffer;
                *pcm_size = size;
                LOGI("decode frame, size = %ld", pcm_size);
                return 0;
            }

            /*re = avcodec_send_packet(pCodecCtx, packet);
            av_packet_unref(packet);
            if (re != 0)
            {
                LOGE("send packet failed, re = %d\n", re);
                //continue;
            }
            re = avcodec_receive_frame(pCodecCtx, frame);
            if (re != 0)
            {
                LOGE("解码失败..., re = %d\n", re);
            }
            else
            {
                LOGI("receive packet success...");
                swr_convert(swrContext, &out_buffer, 44100 * 2, (const uint8_t **) frame->data,
                            frame->nb_samples);
                //缓冲区大小
                int size = av_samples_get_buffer_size(NULL, out_channel_nb, frame->nb_samples,
                                                      AV_SAMPLE_FMT_S16, 1);
                *pcm = out_buffer;
                *pcm_size = size;
                break;
            }*/

            /*avcodec_decode_audio4(pCodecCtx, frame, &got_frame, packet);
            if (got_frame)
            {
                LOGI("解码...");

                swr_convert(swrContext, &out_buffer, 44100 * 2, (const uint8_t **) frame->data,
                            frame->nb_samples);
                //缓冲区大小
                int size = av_samples_get_buffer_size(NULL, out_channel_nb, frame->nb_samples,
                                                      AV_SAMPLE_FMT_S16, 1);
                *pcm = out_buffer;
                *pcm_size = size;
                break;
            }*/
        }
    }
    return 0;
}

void releaseFFmpeg()
{
    av_free_packet(packet);
    av_free(out_buffer);
    av_frame_free(&frame);
    swr_free(&swrContext);
    avcodec_close(pCodecCtx);
    avformat_close_input(&pFormatCtx);
}



