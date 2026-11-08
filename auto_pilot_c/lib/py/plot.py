import numpy as np
from matplotlib import pyplot
import time
import datetime

class TiroPlotPoint:

    color = None
    marker = None
    x = None
    y = None
    _x = None
    _y = None
    plot_point = None
    temporary = False
    def __init__(self, color='red', marker='x', temporary=False):
        self.color = color
        self.marker = marker
        self.temporary = temporary
        pass

    @property
    def point(self):
        return (self.x, self.y)

    @point.setter
    def point(self, point):
        self.x = point[0]
        self.y = point[1]

    def set_last_point(self, plot_point):
        self._x = self.x
        self._y = self.y
        # if self.plot_point:
        #     self.plot_point.set_visible(False)
        self.plot_point = plot_point

    def update(self):
        if self.plot_point and self.temporary:
            self.plot_point.set_data(self.x, self.y)
            return True
        return False

    def loadFromDB(self, db):
        data = db.readCarData(mission=1)
        array = np.asarray(data)
        area = [array[:,4].min(), array[:,4].max(), array[:,3].min(), array[:,3].max()]
        return np.asarray([array[:,3], array[:,4]]), area


class TiroPlot:
    figure = None
    axes = None
    bg = None
    counter = 0
    def __init__(self, xmin=0, xmax=1000, ymin=0, ymax=1000):
        self.figure, self.axes = pyplot.subplots()
        self.axes.axis([xmin, xmax, ymin, ymax])
        pyplot.show(block=False)
        pyplot.pause(0.1)
        self.bg = self.figure.canvas.copy_from_bbox(self.figure.bbox)

    def add(self, point, line=False):
        # self.counter += 1
        # print('update', self.counter)
        # return

        if point.update() == False:
            _point, = self.axes.plot(point.x, point.y, color=point.color, marker=point.marker, animated=True)
            self.axes.draw_artist(_point)
            # if line and 1==2:
            #     if point._x:
            #         self.axes.plot([point._x, point.x], [point._y, point.y], color=point.color)
            point.set_last_point(_point)


        else:

            _point = point.plot_point
        self.axes.draw_artist(_point)

        return _point

    def render(self):
        self.figure.canvas.blit(self.figure.bbox)
        # self.figure.flush_events()
        pyplot.pause(.01)

    def draw(self, points, color='red'):
        # array = np.loadtxt(filename, delimiter = ",")
        # print(points[0,:])
        self.axes.plot(points[1, :], points[0, :], color=color, marker='*')
        pyplot.pause(.01)

    def drawDB(self, db, mission=1, color = 'red'):
        pass
        # self.axes.plot(array[:, 3], array[:, 4], color=color, marker='*')
        # pyplot.pause(.01)

# t1 = datetime.datetime.now()


# plot.drawFile('points.txt', 'yellow')

# p1 = TiroPlotPoint('blue')

# # p2 = TiroPlotPoint(marker='o')
# # p3 = TiroPlotPoint('green', marker='*')
# # p4 = TiroPlotPoint('green', marker='*')
# # p5 = TiroPlotPoint('green', marker='*')
# # p6 = TiroPlotPoint('green', marker='*')
# x = 1
# while x < 1000:

#     p1.point = (x, x)
#     plot.add(p1)


#     p2.point = (x+50, x)
#     plot.add(p2)

#     p3.point = (x+100, x)
#     plot.add(p3)

#     p4.point = (x+150, x)
#     plot.add(p4, True)

#     p5.point = (x+200, x)
#     plot.add(p5)

#     p6.point = (x+250, x)
#     plot.add(p6)

#     # plot.draw()


#     x += 50

#     # print(x)
#     # x += 10
#     pyplot.pause(.01)

# t2 = datetime.datetime.now()
# t1 = int(t1.strftime("%s"))
# t2 = int(t2.strftime("%s"))
# print(t2-t1)
