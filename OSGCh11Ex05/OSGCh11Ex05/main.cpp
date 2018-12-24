#include <Windows.h>
#include <iostream>
#include <osg/Notify>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <osg/Geometry>
#include <osg/Geode>

#include <osgUtil/Optimizer>

#include <osgDB/Registry>
#include <osgDB/ReadFile>

#include <osgGA/TrackballManipulator>
#include <osgGA/FlightManipulator>
#include <osgGA/DriveManipulator>

#include <osgSim/OverlayNode>

#include <osgText/Font>
#include <osgText/Text>

#include <osgParticle/Particle>
#include <osgParticle/ParticleSystem>
#include <osgParticle/ParticleSystemUpdater>
#include <osgParticle/ModularEmitter>
#include <osgParticle/ModularProgram>
#include <osgParticle/RandomRateCounter>
#include <osgParticle/SectorPlacer>
#include <osgParticle/RadialShooter>
#include <osgParticle/AccelOperator>
#include <osgParticle/FluidFrictionOperator>

#include <osgViewer/Viewer>
#include <iostream>

osg::ref_ptr<osg::Group> createMyParticleScene()
{
	osg::ref_ptr<osg::Group> root = new osg::Group;

	// ��������ϵͳģ��
	osgParticle::Particle ptemplate;
	ptemplate.setLifeTime(2);
	ptemplate.setSizeRange(osgParticle::rangef(0.75f, 3.0f));
	ptemplate.setAlphaRange(osgParticle::rangef(1.0f, 1.0f));
	ptemplate.setColorRange(osgParticle::rangev4(
		osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f),
		osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f)));
	ptemplate.setRadius(0.05f);
	ptemplate.setMass(0.05f);

	// ����ϵͳ
	osg::ref_ptr<osgParticle::ParticleSystem> ps = new osgParticle::ParticleSystem;
	// ���ò���
	// ps->setDefaultAttributes("Images/smoke.rgb", false, false);
	ps->setDefaultAttributes("", false, false);
	ps->setDefaultParticleTemplate(ptemplate);

	// ������
	osg::ref_ptr<osgParticle::ModularEmitter> emitter = new osgParticle::ModularEmitter;
	emitter->setParticleSystem(ps.get());

	// ������
	osg::ref_ptr<osgParticle::RandomRateCounter> counter = new osgParticle::RandomRateCounter;
	// ÿ��������ӵ�����
	counter->setRateRange(100.0f, 100.0f);
	emitter->setCounter(counter.get());

	// ������
	osg::ref_ptr<osgParticle::PointPlacer> placer = new osgParticle::PointPlacer;
	placer->setCenter(osg::Vec3(0.0, 0.0, 0.0));
	emitter->setPlacer(placer.get());

	// ������
	osg::ref_ptr<osgParticle::RadialShooter> shooter = new osgParticle::RadialShooter;
	shooter->setInitialSpeedRange(100, 0);
	emitter->setShooter(shooter.get());

	root->addChild(emitter.get());

	// ��׼��̶���
	osg::ref_ptr<osgParticle::ModularProgram> program = new osgParticle::ModularProgram;
	program->setParticleSystem(ps.get());

	// ������
	osg::ref_ptr<osgParticle::AccelOperator> ap = new osgParticle::AccelOperator;
	ap->setToGravity(1.0f);
	// program->addOperator(ap.get());
	osg::ref_ptr<osgParticle::FluidFrictionOperator> ffo = new osgParticle::FluidFrictionOperator;
	ffo->setFluidToAir();
	// program->addOperator(ffo.get());

	// root->addChild(program.get());

	osg::ref_ptr<osgParticle::ParticleSystemUpdater> psu = new osgParticle::ParticleSystemUpdater;
	psu->addParticleSystem(ps.get());

	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	geode->addDrawable(ps.get());

	root->addChild(geode.get());
	root->addChild(psu.get());

	return root.get();
}

int main(int argc, char** argv)
{
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();
	viewer->setUpViewInWindow(100, 100, 800, 600);
	osg::ref_ptr<osg::Group> root = new osg::Group();

	//�Զ�������ϵͳ���볡��
	root->addChild(createMyParticleScene());

	//�Ż���������
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setSceneData(root.get());

	viewer->realize();

	viewer->run();
	return 0;
}