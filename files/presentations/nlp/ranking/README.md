```bash
pip3 install --user jupyterlab notebook numpy scipy catboost sklearn torch torchvision ipywidgets
jupyter nbextension enable --py widgetsnbextension
```

# Small dataset

[Download](https://www.dropbox.com/s/txnkvtd9qr91k7q/ltr.tiny.zip?dl=0)

docker build -t es-ltr .

docker run -p 9300:80 es-ltr

curl -X PUT http://localhost:9300/homedepot?pretty
curl -X DELETE http://localhost:9300/homedepot?pretty


