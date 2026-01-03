# 模板工程

这是基于 `oneup_core` 核心库的模板项目。

## 使用步骤

1) 修改 `include/oneup_config.h` 中的引脚和参数
2) 确认 `platformio.ini` 里的上传端口
3) 编译并上传

```
pio run -t upload -e esp32dev
```

## 默认按钮行为

- 单击：待机 → 背负 → 跟随 → 示教 → 手拉 → 待机
- 双击：进入归位回放
- 长按：切换 WS2812 灯带

