import catboost
cb_pool_train = catboost.Pool(
    [[0.0], [0.0], [0.0], [1.0]],
    label=[0.0, 0.0, 0.0, 1.0],
    group_id=[1, 1, 1, 1]
)

cb_pool_eval = catboost.Pool(
    [[0.0], [0.0], [0.0], [2.0]],
    label=[0.0, 0.0, 0.0, 1.0],
    group_id=[1, 1, 1, 1]
)
params = {
    "loss_function": "YetiRankPairwise",
    "custom_metric": ["NDCG"]
}
cb_model_ltr = catboost.CatBoost(params=params)
cb_model_ltr.fit(cb_pool_train, eval_set=cb_pool_eval)

