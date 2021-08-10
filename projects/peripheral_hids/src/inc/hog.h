/** @file
 *  @brief HoG Service sample
 */

/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifdef __cplusplus
extern "C" {
#endif

void hog_init(void);


struct hids_report_keyboard{
	uint8_t modifier;
	uint8_t reserved;
	uint8_t keycode[6];
} __packed;

void hog_set_keyboard_input(struct hids_report_keyboard *input_report);

void trigger_report_notification();

#ifdef __cplusplus
}
#endif
