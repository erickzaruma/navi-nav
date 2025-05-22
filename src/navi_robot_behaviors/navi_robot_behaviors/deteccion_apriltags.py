import cv2
import numpy as np
from apriltag import apriltag 

# 1. Cámara
cap = cv2.VideoCapture(0)
if not cap.isOpened():
    raise RuntimeError("No se pudo abrir la cámara")

# 2. Detector
detector = apriltag("tag36h11")
tag = None 

while True:
    ret, frame = cap.read()
    if not ret:
        break

    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    detections = detector.detect(gray)

    if detections:
        nuevo_tag = f"Q{detections[0]['id']}"

        if nuevo_tag != tag:
            tag = nuevo_tag; status = "Encontrado"
            print(tag, status)

        corners = np.rint(detections[0]['lb-rb-rt-lt']).astype(int)
        center = tuple(np.rint(detections[0]['center']).astype(int))

        cv2.polylines(frame, [corners], isClosed=True, color=(0, 255, 0), thickness=2)
        cv2.putText(frame, tag, center, cv2.FONT_HERSHEY_SIMPLEX, 0.8, (0, 0, 255), 2)

    cv2.imshow("AprilTags", frame)

    if cv2.waitKey(1) & 0xFF == ord("q"):
        break

cap.release()
cv2.destroyAllWindows()
