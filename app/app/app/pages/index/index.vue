<!-- <template>
	<view class="wrap">
		<view class="dev-area">

			<view class="dev-cart">
				<view class="">
					<view class="dev-name">led1</view>
					<image class="dev-logo" src="/static/led.png" mode=""></image>
				</view>
				<switch class="small-switch" :checked="led1" @change="onLed1Change" color="#8f8f94"></switch>
			</view>

			<view class="dev-cart">
				<view class="">
					<view class="dev-name">led2</view>
					<image class="dev-logo" src="/static/led.png" mode=""></image>
				</view>
				<switch class="small-switch" :checked="led2" @change="onLed2Change" color="#8f8f94"></switch>
			</view>

			<view class="dev-cart">
				<view class="">
					<view class="dev-name">led3</view>
					<image class="dev-logo" src="/static/led.png" mode=""></image>
				</view>
				<switch class="small-switch" :checked="led3" @change="onLed3Change" color="#8f8f94"></switch>
			</view>

			<view class="dev-cart">
				<view class="">
					<view class="dev-name">led4</view>
					<image class="dev-logo" src="/static/led.png" mode=""></image>
				</view>
				<switch class="small-switch" :checked="led4" @change="onLed4Change" color="#8f8f94"></switch>
			</view>


			<view class="dev-cart2">
				<view class="">
					<view class="dev-name">蜂鸣器音量</view>
					<image class="dev-logo" src="/static/beep.png" mode=""></image>
				</view>
				<view class="dev-data">{{beep}}</view>
			</view>
			<slider :value="Number(beep)" :min="0" :max="100" :step="1" style="width: 600rpx;" show-value
				@change="onBuzzerChange" />

			<view class="dev-cart2">
				<view class="">
					<view class="dev-name">烟雾</view>
					<image class="dev-logo" src="/static/MQ2.png" mode=""></image>
				</view>
				<view class="dev-data">{{mq2}}</view>
			</view>

			<view class="dev-cart2">
				<view class="">
					<view class="dev-name">温度</view>
					<image class="dev-logo" src="/static/t.png" mode=""></image>
				</view>
				<view class="dev-data">{{t_g}}.{{t_d}}</view>
			</view>
			<view class="dev-cart2">
				<view class="">
					<view class="dev-name">湿度</view>
					<image class="dev-logo" src="/static/h.png" mode=""></image>
				</view>
				<view class="dev-data">{{h_g}}.{{h_d}}</view>
			</view>


		</view>


	</view>


</template>

<script>
	const {
		createCommonToken
	} = require("@/key.js")
	export default {
		data() {
			return {
				led1: true,
				led2: true,
				led3: true,
				led4: true,
				beep: '0',
				t_g: '0',
				t_d: '0',
				h_g: '0',
				h_d: '0',
				mq2: '0',
				token: '',
			}
		},
		onLoad() {
			const params = {
				access_key: 'xUG//WsdJx8jLak1eZXt64HX68c5RQtvoUnhTT/nbzs=',
				version: '2022-05-01',
				productid: 'Ob1hpCUIu0',
			}
			this.token = createCommonToken(params);
			console.log(this.token)
		},
		onShow() {
			this.fetchDevDate();
			setInterval(()=>{
			 	this.fetchDevDate();
			 },2000)
		},
		methods: {
			fetchDevDate() {
				uni.request({
					url: 'https://iot-api.heclouds.com/thingmodel/query-device-property',
					method: 'GET',
					data: {
						product_id: 'Ob1hpCUIu0',
						device_name: 'd1'
					},
					header: {
						'authorization': this.token //自定义请求头信息
					},
					success: (res) => {
						console.log(res.data);
						console.log(res.data.data[0].value);
						this.mq2 = res.data.data[0].value;
						this.beep = res.data.data[1].value;
						this.h_d = res.data.data[2].value;
						this.h_g = res.data.data[3].value;
						this.led1 = res.data.data[4].value === 'true';
						this.led2 = res.data.data[5].value === 'true';
						this.led3 = res.data.data[6].value === 'true';
						this.led4 = res.data.data[7].value === 'true';
						this.t_d = res.data.data[8].value;
						this.t_g = res.data.data[9].value;
					}
				});
			},
			setProperty(key, value) {
				const params = {};
				params[key] = value;

				uni.request({
					url: 'https://iot-api.heclouds.com/thingmodel/set-device-property',
					method: 'POST',
					data: {
						product_id: 'Ob1hpCUIu0',
						device_name: 'd1',
						params: params
					},
					header: {
						'authorization': this.token
					},
					success: (res) => {
						const body = res.data;
						if (body && body.code === 0) {
							console.log(`设置 ${key} = ${value} 成功`);
						} else {
							console.error(`设置 ${key} 失败`, body);
						}
					},
					fail: (err) => {
						console.error('网络错误', err);
					}
				});
			},
			onLed1Change(e) {
				const v = e.detail.value;
				this.led1 = v;
				console.log('led1 =', v);
				this.setProperty('led1', v);
			},
			onLed2Change(e) {
				const v = e.detail.value;
				this.led2 = v;
				console.log('led2 =', v);
				this.setProperty('led2', v);
			},
			onLed3Change(e) {
				const v = e.detail.value;
				this.led3 = v;
				console.log('led3 =', v);
				this.setProperty('led3', v);
			},
			onLed4Change(e) {
				const v = e.detail.value;
				this.led4 = v;
				console.log('led4 =', v);
				this.setProperty('led4', v);
			},
			onBuzzerChange(e) {
				const v = e.detail.value;
				this.beep = String(v);
				this.setProperty('buzzer', v);
			}
		}
	}
</script>

<style>
	.wrap {
		padding: 30rpx;
	}

	.dev-area {
		display: flex;
		justify-content: space-between;
		flex-wrap: wrap;

	}

	.dev-cart {
		height: 150rpx;
		width: 320rpx;
		border-radius: 30rpx;
		margin-top: 30rpx;
		display: flex;
		justify-content: space-around;
		align-items: center;
		box-shadow: 0 0 15rpx #ccc;
	}

	.dev-cart2 {
		height: 150rpx;
		width: 640rpx;
		border-radius: 30rpx;
		margin-top: 30rpx;
		display: flex;
		justify-content: space-around;
		align-items: center;
		box-shadow: 0 0 15rpx #ccc;
	}

	.dev-name {
		font-size: 20rpx;
		align-items: center;
		color: #8f8f94;
	}

	.dev-logo {
		width: 70rpx;
		height: 70rpx;
		margin-top: 10rpx;
	}

	.dev-data {
		font-size: 50rpx;
		align-items: center;
		color: #6d6d6d;
	}

	.small-switch {
		transform: scale(0.7);
		transform-origin: right center;
	}
</style> -->
<template>
	<view class="page">
		<view class="wrap">

			<!-- ========== 顶部状态栏 ========== -->
			<view class="header">
				<view class="header-left">
					<view class="title">设备控制面板</view>
					<view class="subtitle">{{ deviceName }} · {{ productId }}</view>
				</view>
				<view class="status" :class="online ? 'is-online' : 'is-offline'">
					<view class="dot"></view>
					<text class="status-text">{{ online ? '在线' : '离线' }}</text>
				</view>
			</view>

			<!-- ========== 时间显示窗口 ========== -->
			<view class="time-panel">
				<view class="time-left">
					<text class="time-label">北京时间</text>
					<text class="time-main">{{ displayTimeText }}</text>
					<text class="time-sub">{{ displayDateText }}</text>
				</view>
				<view class="time-status" :class="timeStatusClass" @click="retrySyncTime">
					<view class="time-status-dot"></view>
					<text class="time-status-text">{{ timeStatusText }}</text>
				</view>
			</view>

			<!-- ========== 紧急模式常驻横幅 ========== -->
			<view class="emergency-banner" v-if="emergencyActive" hover-class="emergency-banner-hover"
				@click="emergencyModalVisible = true">
				<view class="emg-banner-icon">!</view>
				<view class="emg-banner-text">
					<text class="emg-banner-title">紧急模式运行中</text>
					<text class="emg-banner-sub">所有控制已锁定 · 点击查看详情</text>
				</view>
				<text class="emg-banner-arrow">›</text>
			</view>

			<!-- ========== 报警模式常驻横幅 ========== -->
			<view class="alert-banner" v-if="Number(mode) === 3 && !emergencyActive"
				hover-class="alert-banner-hover" @click="openAlert">
				<view class="alert-icon">!</view>
				<view class="alert-text">
					<text class="alert-title">设备处于报警模式</text>
					<text class="alert-sub">请及时检查 · 点击查看详情</text>
				</view>
				<text class="alert-arrow">›</text>
			</view>

			<!-- ========== 工作模式 ========== -->
			<view class="panel" :class="{ 'panel-emergency': emergencyActive }">
				<view class="panel-head">
					<view class="panel-title">工作模式</view>
					<view class="panel-badge" :style="{ color: currentMode.color, background: currentMode.bg }">
						{{ currentMode.label }}
					</view>
				</view>
				<view class="segmented" :class="{ 'segmented-locked': emergencyActive }">
					<view v-for="m in modes" :key="m.value" class="segment"
						:class="{ active: Number(mode) === m.value }" hover-class="segment-hover"
						@click="onModeChange(m.value)">
						<text class="seg-text" :style="Number(mode) === m.value ? { color: m.color } : {}">
							{{ m.label }}
						</text>
					</view>
				</view>
				<view class="mode-tip" v-if="emergencyActive">紧急模式下已锁定，等待设备退出</view>
				<view class="mode-tip" v-else>点击切换模式，指令立即下发</view>
			</view>

			<!-- ========== 开关控制 ========== -->
			<view class="panel">
				<view class="panel-title">开关控制</view>
				<view class="led-row" v-for="led in leds" :key="led.key">
					<view class="led-left">
						<image class="led-icon" src="/static/led.png" mode="aspectFit"></image>
						<view class="led-meta">
							<text class="led-name">{{ led.name }}</text>
							<text class="led-state" :class="led.on ? 'state-on' : 'state-off'">
								{{ led.on ? '开启' : '关闭' }}
							</text>
						</view>
					</view>
					<view class="led-actions">
						<text class="led-tip" v-if="tips[led.key]">{{ tips[led.key] }}</text>
						<view class="btn btn-open" :class="{ 'btn-disabled': emergencyActive }"
							hover-class="btn-hover" @click="controlLed(led.key, true)">打开</view>
						<view class="btn btn-close" :class="{ 'btn-disabled': emergencyActive }"
							hover-class="btn-hover" @click="controlLed(led.key, false)">关闭</view>
					</view>
				</view>
			</view>

			<!-- ========== 蜂鸣器音量 ========== -->
			<view class="panel">
				<view class="panel-title">
					蜂鸣器音量
					<text class="lock-tag" v-if="emergencyActive">已锁定</text>
				</view>
				<view class="slider-row">
					<slider class="slider" :value="Number(buzzer)" :min="0" :max="100" :step="1"
						:disabled="emergencyActive" activeColor="#a855f7" block-size="20"
						@change="onBuzzerChange" />
					<text class="slider-val">{{ buzzer }}</text>
				</view>
			</view>

			<!-- ========== 物模型数据卡片 ========== -->
			<view class="section-title">物模型数据</view>
			<view class="cards">
				<view class="card" v-for="c in cards" :key="c.key" hover-class="card-hover"
					@click="openHistory(c)">
					<view class="card-head">
						<view class="card-icon" :style="{ background: c.bg }">
							<image class="card-img" :src="c.icon" mode="aspectFit"></image>
						</view>
						<text class="card-name">{{ c.name }}</text>
					</view>
					<view class="card-value">
						<text class="val">{{ cardValue(c) }}</text>
						<text class="unit" v-if="c.unit">{{ c.unit }}</text>
					</view>
					<view class="card-foot">点击查看历史数据 ›</view>
				</view>
			</view>

			<!-- ========== 紧急记录入口 ========== -->
			<view class="emg-entry" v-if="emergencyHistory.length && !emergencyActive"
				hover-class="card-hover" @click="showEmergencyHistory = true">
				<view class="emg-entry-dot">!</view>
				<view class="emg-entry-text">
					<text class="emg-entry-title">紧急记录</text>
					<text class="emg-entry-sub">共 {{ emergencyHistory.length }} 条，最近：{{ latestEmergencyText }}</text>
				</view>
				<text class="emg-entry-arrow">›</text>
			</view>

			<view class="refresh-tip">
				数据每 {{ emergencyActive ? 2 : 5 }} 秒自动刷新
			</view>
		</view>

		<!-- ================= 历史数据弹窗 ================= -->
		<view class="mask" v-if="showHistory" @click="closeHistory" @touchmove.stop.prevent>
			<view class="modal" @click.stop>
				<view class="modal-head">
					<view class="modal-head-left">
						<text class="modal-title">{{ historyTitle }}</text>
						<text class="modal-sub">最近 24 小时 {{ historyUnit }}</text>
					</view>
					<view class="modal-close" @click="closeHistory">×</view>
				</view>
				<view class="modal-body">
					<view v-if="historyLoading" class="empty">加载中…</view>
					<view v-else-if="!historyBars.length" class="empty">暂无历史数据</view>
					<view v-else>
						<view class="stats">
							<view class="stat">
								<text class="stat-k">最大</text>
								<text class="stat-v">{{ stats.max }}</text>
							</view>
							<view class="stat">
								<text class="stat-k">最小</text>
								<text class="stat-v">{{ stats.min }}</text>
							</view>
							<view class="stat">
								<text class="stat-k">平均</text>
								<text class="stat-v">{{ stats.avg }}</text>
							</view>
							<view class="stat">
								<text class="stat-k">最新</text>
								<text class="stat-v">{{ stats.last }}</text>
							</view>
						</view>
						<view class="chart">
							<view class="bar-col" v-for="(b, i) in historyBars" :key="i">
								<view class="bar" :style="{ height: b.h + '%', background: historyColor }"></view>
							</view>
						</view>
						<view class="chart-axis">
							<text class="axis-t">{{ axisStart }}</text>
							<text class="axis-t">{{ axisEnd }}</text>
						</view>
					</view>
				</view>
			</view>
		</view>

		<!-- ================= 报警模式弹窗 ================= -->
		<view class="mask alert-mask" v-if="showAlert" @touchmove.stop.prevent>
			<view class="alert-modal">
				<view class="alert-modal-icon">
					<text class="alert-modal-icon-txt">!</text>
				</view>
				<text class="alert-modal-title">报警模式已启动</text>
				<text class="alert-modal-desc">
					设备当前工作模式已切换为「报警」，请及时检查设备状态与现场环境。
				</text>
				<view class="alert-modal-info">
					<view class="info-row">
						<text class="info-k">设备</text>
						<text class="info-v">{{ deviceName }}</text>
					</view>
					<view class="info-row">
						<text class="info-k">温度</text>
						<text class="info-v">{{ t_g }}.{{ t_d }} ℃</text>
					</view>
					<view class="info-row">
						<text class="info-k">湿度</text>
						<text class="info-v">{{ h_g }}.{{ h_d }} %RH</text>
					</view>
					<view class="info-row">
						<text class="info-k">烟雾</text>
						<text class="info-v">{{ mq2 }}</text>
					</view>
				</view>
				<view class="alert-modal-btns">
					<view class="alert-btn alert-btn-ghost" hover-class="btn-hover"
						@click="dismissAlert(false)">稍后提醒</view>
					<view class="alert-btn alert-btn-primary" hover-class="btn-hover"
						@click="dismissAlert(true)">知道了</view>
				</view>
			</view>
		</view>

		<!-- ================= 紧急模式弹窗 ================= -->
		<view class="mask emergency-mask" v-if="emergencyModalVisible" @touchmove.stop.prevent>
			<view class="emergency-modal">
				<view class="emg-head">
					<view class="emg-icon">!</view>
					<view class="emg-head-text">
						<text class="emg-title">紧急模式</text>
						<text class="emg-sub">设备已进入紧急状态，等待设备自行退出</text>
					</view>
				</view>

				<view class="emg-duration">
					<text class="emg-duration-label">已持续</text>
					<text class="emg-duration-val">{{ emergencyDurationText }}</text>
				</view>

				<view class="emg-info">
					<view class="info-row">
						<text class="info-k">开始时间</text>
						<text class="info-v">{{ emergencyStartText }}</text>
					</view>
					<view class="info-row">
						<text class="info-k">温度</text>
						<text class="info-v">{{ t_g }}.{{ t_d }} ℃</text>
					</view>
					<view class="info-row">
						<text class="info-k">湿度</text>
						<text class="info-v">{{ h_g }}.{{ h_d }} %RH</text>
					</view>
					<view class="info-row">
						<text class="info-k">烟雾</text>
						<text class="info-v">{{ mq2 }}</text>
					</view>
				</view>

				<view class="emg-history-title">紧急记录（最近 5 条）</view>
				<scroll-view scroll-y class="emg-history-list">
					<view class="emg-record" v-for="(h, i) in emergencyHistory.slice(0, 5)" :key="i">
						<view class="eh-dot" :class="{ 'eh-dot-ongoing': !h.end }"></view>
						<view class="eh-text">
							<text class="eh-time">{{ formatDateTime(h.start) }}</text>
							<text class="eh-dur">{{ h.end ? formatDuration(h.duration) : '进行中' }}</text>
						</view>
					</view>
				</scroll-view>

				<view class="emg-btns">
					<view class="emg-btn" hover-class="btn-hover"
						@click="emergencyModalVisible = false">我知道了</view>
				</view>
			</view>
		</view>

		<!-- ================= 紧急记录弹窗 ================= -->
		<view class="mask" v-if="showEmergencyHistory" @click="showEmergencyHistory = false"
			@touchmove.stop.prevent>
			<view class="modal" @click.stop>
				<view class="modal-head">
					<view class="modal-head-left">
						<text class="modal-title">紧急记录</text>
						<text class="modal-sub">共 {{ emergencyHistory.length }} 条 · 北京时间</text>
					</view>
					<view class="modal-close" @click="showEmergencyHistory = false">×</view>
				</view>
				<scroll-view scroll-y class="modal-body emg-all-list">
					<view v-if="!emergencyHistory.length" class="empty">暂无紧急记录</view>
					<view class="emg-record" v-for="(h, i) in emergencyHistory" :key="i">
						<view class="eh-dot" :class="{ 'eh-dot-ongoing': !h.end }"></view>
						<view class="eh-text">
							<text class="eh-time">{{ formatDateTime(h.start) }}</text>
							<text class="eh-dur">{{ h.end ? formatDuration(h.duration) : '进行中' }}</text>
						</view>
					</view>
				</scroll-view>
			</view>
		</view>
	</view>
</template>

<script>
	const {
		createCommonToken
	} = require("@/key.js")

	const API = {
		deviceDetail: 'https://iot-api.heclouds.com/device/detail',
		getProperty: 'https://iot-api.heclouds.com/thingmodel/query-device-property',
		setProperty: 'https://iot-api.heclouds.com/thingmodel/set-device-property',
		getHistory: 'https://iot-api.heclouds.com/thingmodel/query-device-property-history'
	}

	// ===== 常量 =====
	const EMG_MODE = 6
	const ALERT_MODE = 3
	const MODE4_INTERVAL = 10000              // mode=4 时间戳下发间隔
	const NORMAL_INTERVAL = 5000              // 常规属性轮询
	const EMG_INTERVAL = 2000                 // 紧急模式属性轮询
	const STORAGE_MAX = 50                    // 紧急记录最多保存条数
	const TIME_IDENTIFIER = 'uinx_time'       // ★ 时间戳 identifier
	const BEIJING_OFFSET = 8 * 3600 * 1000

	// ★ 上线时下发时间戳的延迟（毫秒）—— 避开设备刚 CONNECT 未 SUBSCRIBE 的窗口
	const TS_DELAY_1 = 5000
	const TS_DELAY_2 = 8000

	const WEEK_LABELS = ['周日', '周一', '周二', '周三', '周四', '周五', '周六']

	export default {
		data() {
			return {
				productId: 'Ob1hpCUIu0',
				deviceName: 'd1',
				token: '',

				online: false,

				// 开关
				led1: false,
				led2: false,
				led3: false,
				led4: false,
				tips: {
					led1: '',
					led2: '',
					led3: '',
					led4: ''
				},

				// 属性
				mq2: '0',
				buzzer: '0',
				mode: '0',
				t_g: '0',
				t_d: '0',
				h_g: '0',
				h_d: '0',

				// 工作模式枚举（0~4 可由 APP 设置；6=紧急，仅设备可设）
				modes: [{
						value: 0,
						label: '手动',
						color: '#6b7280',
						bg: '#f3f4f6'
					},
					{
						value: 1,
						label: '自动',
						color: '#3b82f6',
						bg: '#eff6ff'
					},
					{
						value: 2,
						label: '节能',
						color: '#10b981',
						bg: '#ecfdf5'
					},
					{
						value: 3,
						label: '报警',
						color: '#ef4444',
						bg: '#fef2f2'
					},
					{
						value: 4,
						label: '睡眠',
						color: '#a855f7',
						bg: '#faf5ff'
					}
				],

				// 数据卡片
				cards: [{
						key: 'mq2',
						name: '烟雾浓度',
						icon: '/static/MQ2.png',
						unit: '',
						ident: 'MQ2',
						color: '#f97316',
						bg: '#fff7ed'
					},
					{
						key: 'temp',
						name: '温度',
						icon: '/static/t.png',
						unit: '℃',
						ident: 't_g',
						color: '#3b82f6',
						bg: '#eff6ff'
					},
					{
						key: 'humi',
						name: '湿度',
						icon: '/static/h.png',
						unit: '%RH',
						ident: 'h_g',
						color: '#22c55e',
						bg: '#f0fdf4'
					},
					{
						key: 'buzzer',
						name: '蜂鸣器音量',
						icon: '/static/beep.png',
						unit: '',
						ident: 'buzzer',
						color: '#a855f7',
						bg: '#faf5ff'
					}
				],

				// 历史数据弹窗
				showHistory: false,
				historyLoading: false,
				historyTitle: '',
				historyUnit: '',
				historyColor: '#3b82f6',
				historyBars: [],
				axisStart: '',
				axisEnd: '',
				stats: {
					max: '-',
					min: '-',
					avg: '-',
					last: '-'
				},

				// 报警模式
				showAlert: false,
				alertAcked: false,
				lastMode: null,

				// 紧急模式
				emergencyActive: false,
				emergencyModalVisible: false,
				emergencySince: 0,
				emergencyHistory: [],
				showEmergencyHistory: false,

				// 时间校准
				timeOffset: 0,
				timeSynced: false,
				timeSyncFailed: false,
				wasOnline: false,

				// 时间显示每秒刷新
				displayNow: Date.now()
			}
		},

		computed: {
			leds() {
				return [{
						key: 'led1',
						name: 'LED 1',
						on: this.led1
					},
					{
						key: 'led2',
						name: 'LED 2',
						on: this.led2
					},
					{
						key: 'led3',
						name: 'LED 3',
						on: this.led3
					},
					{
						key: 'led4',
						name: 'LED 4',
						on: this.led4
					}
				]
			},
			currentMode() {
				if (Number(this.mode) === EMG_MODE) {
					return {
						label: '紧急',
						color: '#ef4444',
						bg: '#fef2f2'
					}
				}
				const m = this.modes.find(x => x.value === Number(this.mode))
				return m || {
					label: '未知',
					color: '#9ca3af',
					bg: '#f3f4f6'
				}
			},

			/* 时间显示 */
			displayTimeText() {
				const d = new Date(this.displayNow + BEIJING_OFFSET)
				const p = n => String(n).padStart(2, '0')
				return `${p(d.getUTCHours())}:${p(d.getUTCMinutes())}:${p(d.getUTCSeconds())}`
			},
			displayDateText() {
				const d = new Date(this.displayNow + BEIJING_OFFSET)
				const p = n => String(n).padStart(2, '0')
				const week = WEEK_LABELS[d.getUTCDay()]
				return `${d.getUTCFullYear()}-${p(d.getUTCMonth()+1)}-${p(d.getUTCDate())} ${week}`
			},
			timeStatusText() {
				if (this.timeSynced) return '已同步'
				if (this.timeSyncFailed) return '获取失败'
				return '同步中...'
			},
			timeStatusClass() {
				if (this.timeSynced) return 'ts-ok'
				if (this.timeSyncFailed) return 'ts-fail'
				return 'ts-pending'
			},

			/* 紧急相关 */
			emergencyDurationText() {
				if (!this.emergencySince) return '-'
				return this.formatDuration(this.displayNow - this.emergencySince)
			},
			emergencyStartText() {
				return this.emergencySince ? this.formatDateTime(this.emergencySince) : '-'
			},
			latestEmergencyText() {
				if (!this.emergencyHistory.length) return ''
				return this.formatDateTime(this.emergencyHistory[0].start)
			}
		},

		onLoad() {
			const params = {
				access_key: 'xUG//WsdJx8jLak1eZXt64HX68c5RQtvoUnhTT/nbzs=',
				version: '2022-05-01',
				productid: this.productId
			}
			try {
				this.token = createCommonToken(params)
			} catch (e) {
				console.error('生成 token 失败', e)
			}

			this.loadEmergencyHistory()
			this.syncNetworkTime()
		},

		onShow() {
			this.startTimer()
			this.startMode4Timer()
			this.startDisplayTick()
		},

		onHide() {
			this.stopTimer()
			this.stopMode4Timer()
			this.stopDisplayTick()
			this._clearOnlineTsTimers()
		},

		onUnload() {
			this.stopTimer()
			this.stopMode4Timer()
			this.stopDisplayTick()
			this._clearOnlineTsTimers()
			if (this._alertRetry) {
				clearTimeout(this._alertRetry)
				this._alertRetry = null
			}
		},

		methods: {
			/* ==================== 通用请求 ==================== */
			request(options) {
				return new Promise((resolve, reject) => {
					uni.request({
						url: options.url,
						method: options.method || 'GET',
						data: options.data || {},
						header: Object.assign({
							authorization: this.token
						}, options.header || {}),
						success: (res) => resolve(res.data),
						fail: (err) => reject(err)
					})
				})
			},

			/* ==================== 时间 ==================== */
			now() {
				return Date.now() + this.timeOffset
			},

			startDisplayTick() {
				this.stopDisplayTick()
				this.displayNow = this.now()
				this._displayTick = setInterval(() => {
					this.displayNow = this.now()
				}, 1000)
			},
			stopDisplayTick() {
				if (this._displayTick) {
					clearInterval(this._displayTick)
					this._displayTick = null
				}
			},

			syncNetworkTime() {
				return new Promise((resolve) => {
					const trySuning = () => {
						uni.request({
							url: 'https://f.m.suning.com/api/ct.do',
							method: 'GET',
							timeout: 3000,
							success: (res) => {
								const d = res.data
								if (d && d.code === '1' && d.currentTime) {
									this.timeOffset = Number(d.currentTime) - Date.now()
									this.timeSynced = true
									this.timeSyncFailed = false
									resolve(true)
								} else {
									tryTaobao()
								}
							},
							fail: tryTaobao
						})
					}
					const tryTaobao = () => {
						uni.request({
							url: 'http://api.m.taobao.com/rest/api3.do?api=mtop.common.getTimestamp',
							method: 'GET',
							timeout: 3000,
							success: (res) => {
								const t = res.data && res.data.data && res.data.data.t
								if (t) {
									this.timeOffset = Number(t) - Date.now()
									this.timeSynced = true
									this.timeSyncFailed = false
									resolve(true)
								} else {
									if (!this.timeSynced) this.timeSyncFailed = true
									resolve(false)
								}
							},
							fail: () => {
								if (!this.timeSynced) this.timeSyncFailed = true
								resolve(false)
							}
						})
					}
					trySuning()
				})
			},

			/* 点击状态标签：
			 * - 未同步：重试网络时间
			 * - 已同步：手动向设备补发时间戳
			 */
			retrySyncTime() {
				if (this.timeSynced) {
					if (!this.online) {
						uni.showToast({
							title: '设备离线',
							icon: 'none'
						})
						return
					}
					this.sendTimestampToDevice('手动补发')
					uni.showToast({
						title: '已补发',
						icon: 'none'
					})
					return
				}
				this.timeSyncFailed = false
				this.syncNetworkTime()
			},

			sendTimestampToDevice(reason) {
				const ts = this.now()
				if (ts - (this._lastTsSentAt || 0) < 800) return
				this._lastTsSentAt = ts
				this.setProperty(TIME_IDENTIFIER, ts)
				console.log(`[时间校准] ${reason} → ${ts} (${this.formatDateTime(ts)})`)
			},

			/* ★ 设备上线：延迟 5s 首发 + 8s 补发，避开设备刚 CONNECT 未 SUBSCRIBE 的窗口 */
			onDeviceOnline() {
				this.syncNetworkTime().then(() => {
					this._clearOnlineTsTimers()

					this._onlineTsTimer1 = setTimeout(() => {
						this._onlineTsTimer1 = null
						if (!this.online) return
						this.sendTimestampToDevice('设备上线校准#1')
					}, TS_DELAY_1)

					this._onlineTsTimer2 = setTimeout(() => {
						this._onlineTsTimer2 = null
						if (!this.online) return
						this.sendTimestampToDevice('设备上线补发#2')
					}, TS_DELAY_2)
				})
			},

			_clearOnlineTsTimers() {
				if (this._onlineTsTimer1) {
					clearTimeout(this._onlineTsTimer1)
					this._onlineTsTimer1 = null
				}
				if (this._onlineTsTimer2) {
					clearTimeout(this._onlineTsTimer2)
					this._onlineTsTimer2 = null
				}
			},

			/* mode=4 每 10s 下发 */
			startMode4Timer() {
				this.stopMode4Timer()
				this._mode4Timer = setInterval(() => {
					if (!this.online) return
					if (Number(this.mode) !== 4) return
					this.sendTimestampToDevice('模式4定期校准')
				}, MODE4_INTERVAL)
			},
			stopMode4Timer() {
				if (this._mode4Timer) {
					clearInterval(this._mode4Timer)
					this._mode4Timer = null
				}
			},

			/* ==================== 动态轮询 ==================== */
			startTimer() {
				this.stopTimer()
				this.refreshAll().finally(() => this._scheduleNext())
			},
			_scheduleNext() {
				if (this.timer) {
					clearTimeout(this.timer)
					this.timer = null
				}
				const delay = this.emergencyActive ? EMG_INTERVAL : NORMAL_INTERVAL
				this.timer = setTimeout(() => {
					this.refreshAll().finally(() => this._scheduleNext())
				}, delay)
			},
			stopTimer() {
				if (this.timer) {
					clearTimeout(this.timer)
					this.timer = null
				}
			},
			refreshAll() {
				return Promise.all([
					this.fetchDeviceStatus(),
					this.fetchProperties()
				])
			},

			/* ==================== 在线状态 ==================== */
			async fetchDeviceStatus() {
				try {
					const res = await this.request({
						url: API.deviceDetail,
						data: {
							product_id: this.productId,
							device_name: this.deviceName
						}
					})
					if (res && res.code === 0 && res.data) {
						const nowOnline = res.data.status === 1
						const wasOnline = this.wasOnline
						this.online = nowOnline
						if (nowOnline && !wasOnline) {
							this.onDeviceOnline()
						}
						this.wasOnline = nowOnline
					}
				} catch (e) {
					console.warn('获取在线状态失败', e)
				}
			},

			/* ==================== 获取属性 ==================== */
			async fetchProperties() {
				try {
					const res = await this.request({
						url: API.getProperty,
						data: {
							product_id: this.productId,
							device_name: this.deviceName
						}
					})
					if (!res || res.code !== 0 || !Array.isArray(res.data)) return

					const list = res.data
					const map = {}
					list.forEach(item => {
						if (item && item.identifier) map[item.identifier] = item.value
					})

					const pick = (idents, idx, def) => {
						const arr = Array.isArray(idents) ? idents : [idents]
						for (const id of arr) {
							if (map[id] !== undefined && map[id] !== null) return map[id]
						}
						if (list[idx] && list[idx].value !== undefined) return list[idx].value
						return def
					}

					this.mq2 = String(pick(['MQ2', 'mq2'], 0, this.mq2))
					this.buzzer = String(pick('buzzer', 1, this.buzzer))
					this.h_d = String(pick('h_d', 2, this.h_d))
					this.h_g = String(pick('h_g', 3, this.h_g))
					this.led1 = String(pick('led1', 4, 'false')) === 'true'
					this.led2 = String(pick('led2', 5, 'false')) === 'true'
					this.led3 = String(pick('led3', 6, 'false')) === 'true'
					this.led4 = String(pick('led4', 7, 'false')) === 'true'
					this.mode = String(pick('mode', 8, this.mode))
					this.t_d = String(pick('t_d', 9, this.t_d))
					this.t_g = String(pick('t_g', 10, this.t_g))

					this.checkAlertMode(this.mode)
					this.checkEmergencyMode(this.mode)
				} catch (e) {
					console.warn('获取属性失败', e)
				}
			},

			/* ==================== 设置属性 ==================== */
			setProperty(key, value) {
				return new Promise((resolve) => {
					uni.request({
						url: API.setProperty,
						method: 'POST',
						data: {
							product_id: this.productId,
							device_name: this.deviceName,
							params: {
								[key]: value
							}
						},
						header: {
							authorization: this.token
						},
						success: (res) => {
							console.log(`设置 ${key} = ${value}`, res.data)
							resolve(res.data)
						},
						fail: (err) => {
							console.warn(`设置 ${key} 失败`, err)
							resolve(null)
						}
					})
				})
			},

			/* ==================== 开关控制 ==================== */
			controlLed(key, value) {
				if (this.emergencyActive) {
					uni.showToast({
						title: '紧急模式下已锁定',
						icon: 'none'
					})
					return
				}
				this.flashTip(key, '已执行')
				this[key] = value
				this.setProperty(key, value)
			},
			flashTip(key, text) {
				this.tips[key] = text
				if (!this._tipTimers) this._tipTimers = {}
				if (this._tipTimers[key]) clearTimeout(this._tipTimers[key])
				this._tipTimers[key] = setTimeout(() => {
					this.tips[key] = ''
				}, 1500)
			},

			/* ==================== 工作模式 ==================== */
			onModeChange(v) {
				if (this.emergencyActive) {
					uni.showToast({
						title: '紧急模式下无法切换',
						icon: 'none'
					})
					return
				}
				if (Number(this.mode) === v) return
				this.mode = String(v)
				this.checkAlertMode(this.mode)
				this.setProperty('mode', v)
			},

			/* ==================== 蜂鸣器 ==================== */
			onBuzzerChange(e) {
				if (this.emergencyActive) return
				const v = e.detail.value
				this.buzzer = String(v)
				this.setProperty('buzzer', v)
			},

			/* ==================== 报警提醒 ==================== */
			checkAlertMode(newMode) {
				const isAlert = Number(newMode) === ALERT_MODE
				const wasAlert = Number(this.lastMode) === ALERT_MODE
				if (isAlert && !wasAlert) {
					this.alertAcked = false
					this.showAlert = true
				}
				if (!isAlert) {
					this.showAlert = false
					this.alertAcked = false
					if (this._alertRetry) {
						clearTimeout(this._alertRetry)
						this._alertRetry = null
					}
				}
				this.lastMode = newMode
			},
			dismissAlert(ack) {
				this.showAlert = false
				if (ack) {
					this.alertAcked = true
				} else {
					this.alertAcked = true
					if (this._alertRetry) clearTimeout(this._alertRetry)
					this._alertRetry = setTimeout(() => {
						this._alertRetry = null
						if (Number(this.mode) === ALERT_MODE) {
							this.alertAcked = false
							this.showAlert = true
						}
					}, 60000)
				}
			},
			openAlert() {
				this.showAlert = true
			},

			/* ==================== 紧急模式 ==================== */
			checkEmergencyMode(newMode) {
				const isEmg = Number(newMode) === EMG_MODE
				const wasEmg = this.emergencyActive
				if (isEmg && !wasEmg) {
					this.emergencyActive = true
					this.enterEmergency()
				} else if (!isEmg && wasEmg) {
					this.emergencyActive = false
					this.exitEmergency()
					this.emergencyModalVisible = false
				}
			},
			enterEmergency() {
				let ongoing = this.emergencyHistory.find(h => h.end === null)
				if (!ongoing) {
					ongoing = {
						start: this.now(),
						end: null,
						duration: null
					}
					this.emergencyHistory.unshift(ongoing)
					this._saveEmergencyHistory()
				}
				this.emergencySince = ongoing.start
				this.emergencyModalVisible = true
			},
			exitEmergency() {
				const ongoing = this.emergencyHistory.find(h => h.end === null)
				if (ongoing) {
					ongoing.end = this.now()
					ongoing.duration = ongoing.end - ongoing.start
					this._saveEmergencyHistory()
				}
				this.emergencySince = 0
			},
			loadEmergencyHistory() {
				try {
					const key = 'emergency_history_' + this.deviceName
					const raw = uni.getStorageSync(key)
					const arr = raw ? JSON.parse(raw) : []
					this.emergencyHistory = Array.isArray(arr) ? arr : []
				} catch (e) {
					this.emergencyHistory = []
				}
			},
			_saveEmergencyHistory() {
				if (this.emergencyHistory.length > STORAGE_MAX) {
					this.emergencyHistory = this.emergencyHistory.slice(0, STORAGE_MAX)
				}
				try {
					const key = 'emergency_history_' + this.deviceName
					uni.setStorageSync(key, JSON.stringify(this.emergencyHistory))
				} catch (e) {
					console.warn('保存紧急记录失败', e)
				}
			},

			/* ==================== 卡片取值 ==================== */
			cardValue(card) {
				switch (card.key) {
					case 'mq2':
						return this.mq2
					case 'temp':
						return `${this.t_g}.${this.t_d}`
					case 'humi':
						return `${this.h_g}.${this.h_d}`
					case 'buzzer':
						return this.buzzer
					default:
						return '-'
				}
			},

			/* ==================== 历史数据 ==================== */
			async openHistory(card) {
				this.showHistory = true
				this.historyTitle = card.name
				this.historyUnit = card.unit ? `(${card.unit})` : ''
				this.historyColor = card.color
				this.historyLoading = true
				this.historyBars = []
				this.axisStart = ''
				this.axisEnd = ''
				this.stats = {
					max: '-',
					min: '-',
					avg: '-',
					last: '-'
				}

				try {
					const endTime = this.now()
					const startTime = endTime - 24 * 60 * 60 * 1000
					const res = await this.request({
						url: API.getHistory,
						data: {
							product_id: this.productId,
							device_name: this.deviceName,
							identifier: card.ident,
							start_time: startTime,
							end_time: endTime,
							limit: 100
						}
					})

					if (!res || res.code !== 0) {
						this.historyLoading = false
						return
					}

					let list = (res.data && res.data.list) || []
					list.sort((a, b) => Number(a.time) - Number(b.time))

					const MAX_POINTS = 30
					if (list.length > MAX_POINTS) {
						const step = list.length / MAX_POINTS
						const sampled = []
						for (let i = 0; i < MAX_POINTS; i++) {
							sampled.push(list[Math.floor(i * step)])
						}
						list = sampled
					}

					const pts = list.map(it => ({
							t: this.formatShortTime(it.time),
							v: parseFloat(it.value)
						}))
						.filter(p => !isNaN(p.v))

					if (!pts.length) {
						this.historyLoading = false
						return
					}

					const vals = pts.map(p => p.v)
					const mx = Math.max.apply(null, vals)
					const mn = Math.min.apply(null, vals)
					const span = (mx - mn) || 1

					this.historyBars = pts.map(p => ({
						h: 15 + ((p.v - mn) / span) * 85
					}))
					this.axisStart = pts[0].t
					this.axisEnd = pts[pts.length - 1].t

					const sum = vals.reduce((a, b) => a + b, 0)
					this.stats = {
						max: this.fmtNum(mx),
						min: this.fmtNum(mn),
						avg: this.fmtNum(sum / vals.length),
						last: this.fmtNum(vals[vals.length - 1])
					}
				} catch (e) {
					console.warn('获取历史数据失败', e)
				} finally {
					this.historyLoading = false
				}
			},
			closeHistory() {
				this.showHistory = false
			},

			/* ==================== 时间格式化（北京时间） ==================== */
			formatDateTime(ts) {
				if (!ts) return '-'
				const d = new Date(ts + BEIJING_OFFSET)
				const p = n => String(n).padStart(2, '0')
				return `${d.getUTCFullYear()}-${p(d.getUTCMonth()+1)}-${p(d.getUTCDate())} ` +
					`${p(d.getUTCHours())}:${p(d.getUTCMinutes())}:${p(d.getUTCSeconds())}`
			},
			formatShortTime(t) {
				let ts = typeof t === 'number' ? t : Number(t)
				if (isNaN(ts)) ts = new Date(t).getTime()
				const d = new Date(ts + BEIJING_OFFSET)
				const p = n => String(n).padStart(2, '0')
				return `${p(d.getUTCMonth()+1)}-${p(d.getUTCDate())} ${p(d.getUTCHours())}:${p(d.getUTCMinutes())}`
			},
			formatDuration(ms) {
				if (!ms || ms < 0) return '-'
				const s = Math.floor(ms / 1000)
				const h = Math.floor(s / 3600)
				const m = Math.floor((s % 3600) / 60)
				const ss = s % 60
				if (h > 0) return `${h}小时${m}分${ss}秒`
				if (m > 0) return `${m}分${ss}秒`
				return `${ss}秒`
			},
			fmtNum(n) {
				if (n === undefined || n === null || isNaN(n)) return '-'
				return Number.isInteger(n) ? String(n) : n.toFixed(1)
			}
		}
	}
</script>

<style>
	/* ================= 基础 ================= */
	.page {
		min-height: 100vh;
		background: #f5f7fa;
		padding: 24rpx 0 60rpx;
		box-sizing: border-box;
		font-family: -apple-system, "PingFang SC", "Microsoft YaHei", sans-serif;
	}

	.wrap {
		width: 100%;
		max-width: 750rpx;
		margin: 0 auto;
		padding: 0 24rpx;
		box-sizing: border-box;
	}

	/* ================= 顶部 ================= */
	.header {
		display: flex;
		justify-content: space-between;
		align-items: center;
		background: #fff;
		border-radius: 24rpx;
		padding: 28rpx 30rpx;
		box-shadow: 0 4rpx 20rpx rgba(0, 0, 0, 0.05);
	}

	.title {
		font-size: 32rpx;
		font-weight: 700;
		color: #1f2937;
	}

	.subtitle {
		font-size: 22rpx;
		color: #9ca3af;
		margin-top: 6rpx;
	}

	.status {
		display: flex;
		align-items: center;
		padding: 8rpx 20rpx;
		border-radius: 999rpx;
	}

	.dot {
		width: 14rpx;
		height: 14rpx;
		border-radius: 50%;
		margin-right: 10rpx;
	}

	.status-text {
		font-size: 22rpx;
		font-weight: 600;
	}

	.is-online {
		background: #ecfdf5;
	}

	.is-online .dot {
		background: #10b981;
		box-shadow: 0 0 0 4rpx rgba(16, 185, 129, 0.15);
	}

	.is-online .status-text {
		color: #059669;
	}

	.is-offline {
		background: #f3f4f6;
	}

	.is-offline .dot {
		background: #9ca3af;
	}

	.is-offline .status-text {
		color: #6b7280;
	}

	/* ================= 时间窗口 ================= */
	.time-panel {
		display: flex;
		justify-content: space-between;
		align-items: center;
		background: linear-gradient(135deg, #1f2937 0%, #111827 100%);
		border-radius: 24rpx;
		padding: 28rpx 30rpx;
		margin-top: 24rpx;
		box-shadow: 0 6rpx 24rpx rgba(17, 24, 39, 0.25);
	}

	.time-left {
		display: flex;
		flex-direction: column;
	}

	.time-label {
		font-size: 22rpx;
		color: rgba(255, 255, 255, 0.55);
		letter-spacing: 2rpx;
	}

	.time-main {
		font-size: 60rpx;
		font-weight: 700;
		color: #fff;
		line-height: 1.05;
		margin-top: 8rpx;
		font-variant-numeric: tabular-nums;
		letter-spacing: 2rpx;
	}

	.time-sub {
		font-size: 22rpx;
		color: rgba(255, 255, 255, 0.55);
		margin-top: 10rpx;
	}

	.time-status {
		display: flex;
		align-items: center;
		padding: 10rpx 20rpx;
		border-radius: 999rpx;
		font-size: 20rpx;
		font-weight: 600;
	}

	.time-status-dot {
		width: 12rpx;
		height: 12rpx;
		border-radius: 50%;
		margin-right: 10rpx;
		background: currentColor;
	}

	.time-status-text {
		color: inherit;
	}

	.ts-ok {
		background: rgba(16, 185, 129, 0.18);
		color: #34d399;
	}

	.ts-fail {
		background: rgba(239, 68, 68, 0.18);
		color: #f87171;
	}

	.ts-pending {
		background: rgba(156, 163, 175, 0.18);
		color: #d1d5db;
	}

	/* ================= 紧急横幅 ================= */
	.emergency-banner {
		display: flex;
		align-items: center;
		margin-top: 24rpx;
		padding: 22rpx 26rpx;
		background: linear-gradient(135deg, #ef4444 0%, #b91c1c 100%);
		border-radius: 24rpx;
		animation: emergencyPulse 1.5s ease-in-out infinite;
	}

	.emergency-banner-hover {
		opacity: 0.9;
	}

	@keyframes emergencyPulse {
		0%, 100% {
			box-shadow: 0 0 0 0 rgba(239, 68, 68, 0.6);
		}

		50% {
			box-shadow: 0 0 0 16rpx rgba(239, 68, 68, 0);
		}
	}

	.emg-banner-icon {
		width: 52rpx;
		height: 52rpx;
		border-radius: 50%;
		background: #fff;
		color: #ef4444;
		font-size: 34rpx;
		font-weight: 900;
		line-height: 52rpx;
		text-align: center;
		margin-right: 18rpx;
		flex-shrink: 0;
	}

	.emg-banner-text {
		flex: 1;
		display: flex;
		flex-direction: column;
	}

	.emg-banner-title {
		font-size: 28rpx;
		font-weight: 700;
		color: #fff;
	}

	.emg-banner-sub {
		font-size: 20rpx;
		color: rgba(255, 255, 255, 0.85);
		margin-top: 4rpx;
	}

	.emg-banner-arrow {
		font-size: 40rpx;
		color: rgba(255, 255, 255, 0.8);
	}

	/* ================= 报警横幅 ================= */
	.alert-banner {
		display: flex;
		align-items: center;
		margin-top: 24rpx;
		padding: 22rpx 26rpx;
		background: linear-gradient(135deg, #fef2f2 0%, #fee2e2 100%);
		border: 2rpx solid #fecaca;
		border-radius: 24rpx;
		animation: alertPulse 2s ease-in-out infinite;
	}

	.alert-banner-hover {
		opacity: 0.85;
	}

	@keyframes alertPulse {
		0%, 100% {
			box-shadow: 0 0 0 0 rgba(239, 68, 68, 0.4);
		}

		50% {
			box-shadow: 0 0 0 12rpx rgba(239, 68, 68, 0);
		}
	}

	.alert-icon {
		width: 48rpx;
		height: 48rpx;
		border-radius: 50%;
		background: #ef4444;
		color: #fff;
		font-size: 32rpx;
		font-weight: 700;
		line-height: 48rpx;
		text-align: center;
		margin-right: 18rpx;
		flex-shrink: 0;
	}

	.alert-text {
		flex: 1;
		display: flex;
		flex-direction: column;
	}

	.alert-title {
		font-size: 26rpx;
		font-weight: 700;
		color: #b91c1c;
	}

	.alert-sub {
		font-size: 20rpx;
		color: #f87171;
		margin-top: 4rpx;
	}

	.alert-arrow {
		font-size: 36rpx;
		color: #fca5a5;
	}

	/* ================= 面板 ================= */
	.panel {
		background: #fff;
		border-radius: 24rpx;
		padding: 24rpx 26rpx;
		margin-top: 24rpx;
		box-shadow: 0 4rpx 20rpx rgba(0, 0, 0, 0.05);
		transition: all 0.3s;
	}

	.panel-emergency {
		background: linear-gradient(135deg, #fef2f2 0%, #fee2e2 100%);
		border: 2rpx solid #fca5a5;
		box-shadow: 0 4rpx 20rpx rgba(239, 68, 68, 0.15);
	}

	.panel-emergency .panel-title {
		color: #b91c1c;
	}

	.panel-head {
		display: flex;
		justify-content: space-between;
		align-items: center;
		margin-bottom: 18rpx;
	}

	.panel-title {
		font-size: 26rpx;
		font-weight: 600;
		color: #374151;
		display: flex;
		align-items: center;
	}

	.lock-tag {
		font-size: 18rpx;
		color: #ef4444;
		background: #fee2e2;
		padding: 2rpx 12rpx;
		border-radius: 8rpx;
		margin-left: 12rpx;
	}

	.panel-badge {
		font-size: 22rpx;
		font-weight: 600;
		padding: 6rpx 18rpx;
		border-radius: 999rpx;
	}

	/* ================= 分段控制器 ================= */
	.segmented {
		display: flex;
		background: #f3f4f6;
		border-radius: 16rpx;
		padding: 6rpx;
		transition: all 0.3s;
	}

	.segmented-locked {
		pointer-events: none;
		opacity: 0.4;
		filter: grayscale(0.8);
	}

	.segment {
		flex: 1;
		text-align: center;
		padding: 18rpx 0;
		border-radius: 12rpx;
		transition: all 0.2s;
	}

	.segment-hover {
		opacity: 0.7;
	}

	.segment.active {
		background: #fff;
		box-shadow: 0 2rpx 8rpx rgba(0, 0, 0, 0.08);
	}

	.seg-text {
		font-size: 24rpx;
		color: #6b7280;
		font-weight: 500;
	}

	.segment.active .seg-text {
		font-weight: 700;
	}

	.mode-tip {
		font-size: 20rpx;
		color: #c7c9ce;
		margin-top: 16rpx;
		text-align: center;
	}

	/* ================= LED ================= */
	.led-row {
		display: flex;
		justify-content: space-between;
		align-items: center;
		padding: 16rpx 0;
		border-top: 1rpx solid #f3f4f6;
	}

	.led-row:first-of-type {
		border-top: none;
	}

	.led-left {
		display: flex;
		align-items: center;
	}

	.led-icon {
		width: 60rpx;
		height: 60rpx;
		margin-right: 18rpx;
	}

	.led-meta {
		display: flex;
		flex-direction: column;
	}

	.led-name {
		font-size: 26rpx;
		color: #1f2937;
		font-weight: 600;
	}

	.led-state {
		font-size: 20rpx;
		margin-top: 4rpx;
	}

	.state-on {
		color: #10b981;
	}

	.state-off {
		color: #9ca3af;
	}

	.led-actions {
		display: flex;
		align-items: center;
	}

	.led-tip {
		font-size: 20rpx;
		color: #10b981;
		margin-right: 12rpx;
	}

	.btn {
		width: 110rpx;
		height: 54rpx;
		line-height: 54rpx;
		text-align: center;
		border-radius: 27rpx;
		font-size: 22rpx;
		color: #fff;
		margin-left: 12rpx;
		transition: all 0.2s;
	}

	.btn-open {
		background: #10b981;
	}

	.btn-close {
		background: #ef4444;
	}

	.btn-disabled {
		background: #d1d5db !important;
		opacity: 0.6;
	}

	.btn-hover {
		opacity: 0.75;
	}

	/* ================= 蜂鸣器 ================= */
	.slider-row {
		display: flex;
		align-items: center;
	}

	.slider {
		flex: 1;
		margin-right: 20rpx;
	}

	.slider-val {
		width: 70rpx;
		text-align: right;
		font-size: 28rpx;
		color: #a855f7;
		font-weight: 600;
	}

	/* ================= 卡片 ================= */
	.section-title {
		font-size: 26rpx;
		font-weight: 600;
		color: #6b7280;
		margin: 32rpx 0 16rpx 6rpx;
	}

	.cards {
		display: flex;
		flex-wrap: wrap;
		justify-content: space-between;
	}

	.card {
		width: calc(50% - 10rpx);
		box-sizing: border-box;
		background: #fff;
		border-radius: 24rpx;
		padding: 24rpx;
		margin-bottom: 20rpx;
		box-shadow: 0 4rpx 20rpx rgba(0, 0, 0, 0.05);
		transition: all 0.2s;
	}

	.card-hover {
		transform: translateY(-4rpx);
		box-shadow: 0 10rpx 30rpx rgba(0, 0, 0, 0.1);
	}

	.card-head {
		display: flex;
		align-items: center;
		margin-bottom: 18rpx;
	}

	.card-icon {
		width: 52rpx;
		height: 52rpx;
		border-radius: 16rpx;
		display: flex;
		align-items: center;
		justify-content: center;
		margin-right: 12rpx;
	}

	.card-img {
		width: 32rpx;
		height: 32rpx;
	}

	.card-name {
		font-size: 24rpx;
		color: #4b5563;
		font-weight: 600;
	}

	.card-value {
		display: flex;
		align-items: baseline;
	}

	.val {
		font-size: 46rpx;
		font-weight: 700;
		color: #111827;
		line-height: 1.2;
	}

	.unit {
		font-size: 22rpx;
		color: #9ca3af;
		margin-left: 6rpx;
	}

	.card-foot {
		font-size: 20rpx;
		color: #c7c9ce;
		margin-top: 12rpx;
	}

	/* ================= 紧急记录入口 ================= */
	.emg-entry {
		display: flex;
		align-items: center;
		margin-top: 24rpx;
		background: #fff;
		border-radius: 24rpx;
		padding: 22rpx 26rpx;
		box-shadow: 0 4rpx 20rpx rgba(0, 0, 0, 0.05);
	}

	.emg-entry-dot {
		width: 48rpx;
		height: 48rpx;
		border-radius: 50%;
		background: #fef2f2;
		color: #ef4444;
		font-size: 30rpx;
		font-weight: 900;
		line-height: 48rpx;
		text-align: center;
		margin-right: 18rpx;
		flex-shrink: 0;
	}

	.emg-entry-text {
		flex: 1;
		display: flex;
		flex-direction: column;
	}

	.emg-entry-title {
		font-size: 26rpx;
		font-weight: 600;
		color: #1f2937;
	}

	.emg-entry-sub {
		font-size: 20rpx;
		color: #9ca3af;
		margin-top: 4rpx;
	}

	.emg-entry-arrow {
		font-size: 36rpx;
		color: #d1d5db;
	}

	.refresh-tip {
		text-align: center;
		font-size: 20rpx;
		color: #c7c9ce;
		margin-top: 20rpx;
	}

	/* ================= 通用弹窗 ================= */
	.mask {
		position: fixed;
		left: 0;
		right: 0;
		top: 0;
		bottom: 0;
		background: rgba(0, 0, 0, 0.5);
		display: flex;
		align-items: center;
		justify-content: center;
		z-index: 999;
	}

	.modal {
		width: 670rpx;
		max-height: 80vh;
		background: #fff;
		border-radius: 28rpx;
		overflow: hidden;
		display: flex;
		flex-direction: column;
	}

	.modal-head {
		display: flex;
		justify-content: space-between;
		align-items: center;
		padding: 28rpx 30rpx;
		border-bottom: 1rpx solid #f3f4f6;
	}

	.modal-head-left {
		display: flex;
		flex-direction: column;
	}

	.modal-title {
		font-size: 30rpx;
		font-weight: 700;
		color: #1f2937;
	}

	.modal-sub {
		font-size: 20rpx;
		color: #9ca3af;
		margin-top: 6rpx;
	}

	.modal-close {
		width: 56rpx;
		height: 56rpx;
		line-height: 52rpx;
		text-align: center;
		font-size: 40rpx;
		color: #9ca3af;
		border-radius: 50%;
		background: #f3f4f6;
	}

	.modal-body {
		padding: 26rpx 30rpx 34rpx;
		overflow-y: auto;
	}

	.empty {
		text-align: center;
		color: #9ca3af;
		font-size: 24rpx;
		padding: 80rpx 0;
	}

	/* ================= 历史数据 ================= */
	.stats {
		display: flex;
		justify-content: space-between;
		margin-bottom: 26rpx;
	}

	.stat {
		flex: 1;
		display: flex;
		flex-direction: column;
		align-items: center;
		background: #f9fafb;
		border-radius: 16rpx;
		padding: 16rpx 0;
		margin: 0 6rpx;
	}

	.stat-k {
		font-size: 20rpx;
		color: #9ca3af;
	}

	.stat-v {
		font-size: 28rpx;
		font-weight: 700;
		color: #1f2937;
		margin-top: 6rpx;
	}

	.chart {
		height: 320rpx;
		display: flex;
		align-items: flex-end;
		padding: 0 4rpx;
		border-bottom: 2rpx solid #e5e7eb;
	}

	.bar-col {
		flex: 1;
		height: 100%;
		display: flex;
		align-items: flex-end;
		padding: 0 2rpx;
		box-sizing: border-box;
	}

	.bar {
		width: 100%;
		border-radius: 4rpx 4rpx 0 0;
		min-height: 4rpx;
		transition: height 0.3s ease;
	}

	.chart-axis {
		display: flex;
		justify-content: space-between;
		margin-top: 12rpx;
	}

	.axis-t {
		font-size: 20rpx;
		color: #9ca3af;
	}

	/* ================= 报警弹窗 ================= */
	.alert-mask {
		z-index: 1000;
		background: rgba(0, 0, 0, 0.6);
	}

	.alert-modal {
		width: 600rpx;
		background: #fff;
		border-radius: 28rpx;
		padding: 40rpx 34rpx 30rpx;
		display: flex;
		flex-direction: column;
		align-items: center;
		animation: modalPop 0.25s ease;
	}

	@keyframes modalPop {
		0% {
			transform: scale(0.9);
			opacity: 0;
		}

		100% {
			transform: scale(1);
			opacity: 1;
		}
	}

	.alert-modal-icon {
		width: 100rpx;
		height: 100rpx;
		border-radius: 50%;
		background: #fef2f2;
		display: flex;
		align-items: center;
		justify-content: center;
		margin-bottom: 24rpx;
		border: 6rpx solid #fee2e2;
	}

	.alert-modal-icon-txt {
		font-size: 56rpx;
		font-weight: 700;
		color: #ef4444;
	}

	.alert-modal-title {
		font-size: 34rpx;
		font-weight: 700;
		color: #1f2937;
		margin-bottom: 16rpx;
	}

	.alert-modal-desc {
		font-size: 24rpx;
		color: #6b7280;
		line-height: 1.6;
		text-align: center;
		margin-bottom: 26rpx;
	}

	.alert-modal-info {
		width: 100%;
		background: #fef2f2;
		border-radius: 18rpx;
		padding: 16rpx 24rpx;
		margin-bottom: 30rpx;
		box-sizing: border-box;
	}

	.info-row {
		display: flex;
		justify-content: space-between;
		align-items: center;
		padding: 10rpx 0;
	}

	.info-k {
		font-size: 22rpx;
		color: #9ca3af;
	}

	.info-v {
		font-size: 24rpx;
		font-weight: 600;
		color: #b91c1c;
	}

	.alert-modal-btns {
		display: flex;
		width: 100%;
	}

	.alert-btn {
		flex: 1;
		height: 84rpx;
		line-height: 84rpx;
		text-align: center;
		border-radius: 42rpx;
		font-size: 28rpx;
		font-weight: 600;
	}

	.alert-btn-ghost {
		background: #f3f4f6;
		color: #6b7280;
		margin-right: 20rpx;
	}

	.alert-btn-primary {
		background: #ef4444;
		color: #fff;
	}

	/* ================= 紧急弹窗 ================= */
	.emergency-mask {
		z-index: 1001;
		background: rgba(0, 0, 0, 0.7);
	}

	.emergency-modal {
		width: 640rpx;
		max-height: 85vh;
		background: #fff;
		border-radius: 28rpx;
		padding: 34rpx 30rpx 26rpx;
		box-sizing: border-box;
		display: flex;
		flex-direction: column;
		border: 4rpx solid #ef4444;
		animation: modalPop 0.25s ease;
	}

	.emg-head {
		display: flex;
		align-items: center;
		margin-bottom: 22rpx;
	}

	.emg-icon {
		width: 80rpx;
		height: 80rpx;
		border-radius: 50%;
		background: #ef4444;
		color: #fff;
		font-size: 52rpx;
		font-weight: 900;
		line-height: 80rpx;
		text-align: center;
		margin-right: 18rpx;
		flex-shrink: 0;
		animation: emergencyPulse 1.5s ease-in-out infinite;
	}

	.emg-head-text {
		flex: 1;
		display: flex;
		flex-direction: column;
	}

	.emg-title {
		font-size: 34rpx;
		font-weight: 700;
		color: #b91c1c;
	}

	.emg-sub {
		font-size: 20rpx;
		color: #9ca3af;
		margin-top: 4rpx;
	}

	.emg-duration {
		display: flex;
		justify-content: space-between;
		align-items: center;
		background: linear-gradient(135deg, #fef2f2, #fee2e2);
		border-radius: 18rpx;
		padding: 20rpx 26rpx;
		margin-bottom: 20rpx;
	}

	.emg-duration-label {
		font-size: 24rpx;
		color: #b91c1c;
	}

	.emg-duration-val {
		font-size: 34rpx;
		font-weight: 700;
		color: #ef4444;
	}

	.emg-info {
		background: #fef2f2;
		border-radius: 18rpx;
		padding: 12rpx 24rpx;
		margin-bottom: 20rpx;
	}

	.emg-history-title {
		font-size: 24rpx;
		font-weight: 600;
		color: #374151;
		margin-bottom: 12rpx;
	}

	.emg-history-list {
		max-height: 300rpx;
		background: #f9fafb;
		border-radius: 16rpx;
		padding: 12rpx 20rpx;
		margin-bottom: 22rpx;
	}

	.emg-all-list {
		max-height: 65vh;
	}

	.emg-record {
		display: flex;
		align-items: flex-start;
		padding: 14rpx 0;
		border-bottom: 1rpx solid #e5e7eb;
	}

	.emg-record:last-child {
		border-bottom: none;
	}

	.eh-dot {
		width: 16rpx;
		height: 16rpx;
		border-radius: 50%;
		background: #9ca3af;
		margin-top: 10rpx;
		margin-right: 16rpx;
		flex-shrink: 0;
	}

	.eh-dot-ongoing {
		background: #ef4444;
		box-shadow: 0 0 0 6rpx rgba(239, 68, 68, 0.2);
		animation: emergencyPulse 1.5s ease-in-out infinite;
	}

	.eh-text {
		flex: 1;
		display: flex;
		flex-direction: column;
	}

	.eh-time {
		font-size: 24rpx;
		color: #1f2937;
		font-weight: 500;
	}

	.eh-dur {
		font-size: 20rpx;
		color: #9ca3af;
		margin-top: 4rpx;
	}

	.emg-btns {
		display: flex;
	}

	.emg-btn {
		flex: 1;
		height: 84rpx;
		line-height: 84rpx;
		text-align: center;
		border-radius: 42rpx;
		font-size: 28rpx;
		font-weight: 600;
		color: #fff;
		background: #ef4444;
	}
</style>

	.stat-k {
		font-size: 20rpx;
		color: #9ca3af;
	}

	.stat-v {
		font-size: 28rpx;
		font-weight: 700;
		color: #1f2937;
		margin-top: 6rpx;
	}

	.chart {
		height: 320rpx;
		display: flex;
		align-items: flex-end;
		padding: 0 4rpx;
		border-bottom: 2rpx solid #e5e7eb;
	}

	.bar-col {
		flex: 1;
		height: 100%;
		display: flex;
		align-items: flex-end;
		padding: 0 2rpx;
		box-sizing: border-box;
	}

	.bar {
		width: 100%;
		border-radius: 4rpx 4rpx 0 0;
		min-height: 4rpx;
		transition: height 0.3s ease;
	}

	.chart-axis {
		display: flex;
		justify-content: space-between;
		margin-top: 12rpx;
	}

	.axis-t {
		font-size: 20rpx;
		color: #9ca3af;
	}

	/* ================= 报警弹窗 ================= */
	.alert-mask {
		z-index: 1000;
		background: rgba(0, 0, 0, 0.6);
	}

	.alert-modal {
		width: 600rpx;
		background: #fff;
		border-radius: 28rpx;
		padding: 40rpx 34rpx 30rpx;
		display: flex;
		flex-direction: column;
		align-items: center;
		animation: modalPop 0.25s ease;
	}

	@keyframes modalPop {
		0% {
			transform: scale(0.9);
			opacity: 0;
		}

		100% {
			transform: scale(1);
			opacity: 1;
		}
	}

	.alert-modal-icon {
		width: 100rpx;
		height: 100rpx;
		border-radius: 50%;
		background: #fef2f2;
		display: flex;
		align-items: center;
		justify-content: center;
		margin-bottom: 24rpx;
		border: 6rpx solid #fee2e2;
	}

	.alert-modal-icon-txt {
		font-size: 56rpx;
		font-weight: 700;
		color: #ef4444;
	}

	.alert-modal-title {
		font-size: 34rpx;
		font-weight: 700;
		color: #1f2937;
		margin-bottom: 16rpx;
	}

	.alert-modal-desc {
		font-size: 24rpx;
		color: #6b7280;
		line-height: 1.6;
		text-align: center;
		margin-bottom: 26rpx;
	}

	.alert-modal-info {
		width: 100%;
		background: #fef2f2;
		border-radius: 18rpx;
		padding: 16rpx 24rpx;
		margin-bottom: 30rpx;
		box-sizing: border-box;
	}

	.info-row {
		display: flex;
		justify-content: space-between;
		align-items: center;
		padding: 10rpx 0;
	}

	.info-k {
		font-size: 22rpx;
		color: #9ca3af;
	}

	.info-v {
		font-size: 24rpx;
		font-weight: 600;
		color: #b91c1c;
	}

	.alert-modal-btns {
		display: flex;
		width: 100%;
	}

	.alert-btn {
		flex: 1;
		height: 84rpx;
		line-height: 84rpx;
		text-align: center;
		border-radius: 42rpx;
		font-size: 28rpx;
		font-weight: 600;
	}

	.alert-btn-ghost {
		background: #f3f4f6;
		color: #6b7280;
		margin-right: 20rpx;
	}

	.alert-btn-primary {
		background: #ef4444;
		color: #fff;
	}

	/* ================= 紧急弹窗 ================= */
	.emergency-mask {
		z-index: 1001;
		background: rgba(0, 0, 0, 0.7);
	}

	.emergency-modal {
		width: 640rpx;
		max-height: 85vh;
		background: #fff;
		border-radius: 28rpx;
		padding: 34rpx 30rpx 26rpx;
		box-sizing: border-box;
		display: flex;
		flex-direction: column;
		border: 4rpx solid #ef4444;
		animation: modalPop 0.25s ease;
	}

	.emg-head {
		display: flex;
		align-items: center;
		margin-bottom: 22rpx;
	}

	.emg-icon {
		width: 80rpx;
		height: 80rpx;
		border-radius: 50%;
		background: #ef4444;
		color: #fff;
		font-size: 52rpx;
		font-weight: 900;
		line-height: 80rpx;
		text-align: center;
		margin-right: 18rpx;
		flex-shrink: 0;
		animation: emergencyPulse 1.5s ease-in-out infinite;
	}

	.emg-head-text {
		flex: 1;
		display: flex;
		flex-direction: column;
	}

	.emg-title {
		font-size: 34rpx;
		font-weight: 700;
		color: #b91c1c;
	}

	.emg-sub {
		font-size: 20rpx;
		color: #9ca3af;
		margin-top: 4rpx;
	}

	.emg-duration {
		display: flex;
		justify-content: space-between;
		align-items: center;
		background: linear-gradient(135deg, #fef2f2, #fee2e2);
		border-radius: 18rpx;
		padding: 20rpx 26rpx;
		margin-bottom: 20rpx;
	}

	.emg-duration-label {
		font-size: 24rpx;
		color: #b91c1c;
	}

	.emg-duration-val {
		font-size: 34rpx;
		font-weight: 700;
		color: #ef4444;
	}

	.emg-info {
		background: #fef2f2;
		border-radius: 18rpx;
		padding: 12rpx 24rpx;
		margin-bottom: 20rpx;
	}

	.emg-history-title {
		font-size: 24rpx;
		font-weight: 600;
		color: #374151;
		margin-bottom: 12rpx;
	}

	.emg-history-list {
		max-height: 300rpx;
		background: #f9fafb;
		border-radius: 16rpx;
		padding: 12rpx 20rpx;
		margin-bottom: 22rpx;
	}

	.emg-all-list {
		max-height: 65vh;
	}

	.emg-record {
		display: flex;
		align-items: flex-start;
		padding: 14rpx 0;
		border-bottom: 1rpx solid #e5e7eb;
	}

	.emg-record:last-child {
		border-bottom: none;
	}

	.eh-dot {
		width: 16rpx;
		height: 16rpx;
		border-radius: 50%;
		background: #9ca3af;
		margin-top: 10rpx;
		margin-right: 16rpx;
		flex-shrink: 0;
	}

	.eh-dot-ongoing {
		background: #ef4444;
		box-shadow: 0 0 0 6rpx rgba(239, 68, 68, 0.2);
		animation: emergencyPulse 1.5s ease-in-out infinite;
	}

	.eh-text {
		flex: 1;
		display: flex;
		flex-direction: column;
	}

	.eh-time {
		font-size: 24rpx;
		color: #1f2937;
		font-weight: 500;
	}

	.eh-dur {
		font-size: 20rpx;
		color: #9ca3af;
		margin-top: 4rpx;
	}

	.emg-btns {
		display: flex;
	}

	.emg-btn {
		flex: 1;
		height: 84rpx;
		line-height: 84rpx;
		text-align: center;
		border-radius: 42rpx;
		font-size: 28rpx;
		font-weight: 600;
		color: #fff;
		background: #ef4444;
	}
</style>
