# Quick Logo Training Report

## Data

- Cleaned subset: `/home/kazeform/2026upper/datasets/logo_label_450`
- Annotated images kept: 423
- Empty negative images kept: 1
- Empty images deleted: 26
- Training dataset: `/home/kazeform/2026upper/datasets/logo_train_quick`
- Split: 339 train, 85 val
- Classes: `jt`, `zt`, `yd`

## Model

- Base: `models/yolo26s.pt`
- Quick trained model: `/home/kazeform/2026upper/models/logo_yolo26s_quick.pt`
- Training stopped after 10 full epochs because the baseline was already usable.

## Validation

Validation on the held-out labeled split:

| class | precision | recall | mAP50 | mAP50-95 |
| --- | ---: | ---: | ---: | ---: |
| all | 0.850 | 0.900 | 0.961 | 0.649 |
| jt | 0.843 | 0.700 | 0.911 | 0.610 |
| zt | 0.819 | 1.000 | 0.976 | 0.669 |
| yd | 0.888 | 1.000 | 0.995 | 0.667 |

Validation artifacts:

`/home/kazeform/runs/detect/runs/logo_eval/quick_yolo26s_val`

## External Test

External test set:

`/home/kazeform/2026upper/datasets/logo_external_test/images`

This contains 36 original images not used in the 450-image labeling subset.

At confidence 0.25:

- `jt`: 5/12 images detected
- `zt`: 12/12 images detected
- `yd`: 12/12 images detected

At confidence 0.15:

- `jt`: 10/12 images detected
- `zt`: 12/12 images detected
- `yd`: 12/12 images detected

Prediction outputs:

- `/home/kazeform/runs/detect/runs/logo_eval/external_test`
- `/home/kazeform/runs/detect/runs/logo_eval/external_test_conf015`

## Notes

`jt` is the weak class in this quick baseline. Lowering confidence improves recall, so many misses are low-confidence detections rather than complete failures. For the next training pass, prioritize more hard `jt` examples and possibly use a lower runtime threshold for `jt`.
