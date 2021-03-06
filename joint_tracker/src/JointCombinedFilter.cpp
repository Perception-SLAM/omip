#include <boost/foreach.hpp>

#include "joint_tracker/JointCombinedFilter.h"
#include "joint_tracker/JointFilter.h"
#include "joint_tracker/RigidJointFilter.h"
#include "joint_tracker/PrismaticJointFilter.h"
#include "joint_tracker/RevoluteJointFilter.h"
#include "joint_tracker/DisconnectedJointFilter.h"

using namespace omip;
using namespace MatrixWrapper;
using namespace BFL;

JointCombinedFilterId JointCombinedFilter::_joint_id_generator = 1;

JointCombinedFilter::JointCombinedFilter():
    _joint_id(JointCombinedFilter::_joint_id_generator++),
    _normalizing_term(0.25)
{
    this->_joint_filters[DISCONNECTED_JOINT] = DisconnectedJointFilterPtr( new DisconnectedJointFilter());
    this->_joint_filters[RIGID_JOINT] = RigidJointFilterPtr(new RigidJointFilter());
    this->_joint_filters[PRISMATIC_JOINT] = PrismaticJointFilterPtr( new PrismaticJointFilter());
    this->_joint_filters[REVOLUTE_JOINT] =  RevoluteJointFilterPtr( new RevoluteJointFilter());

    BOOST_FOREACH(joint_filters_map::value_type joint_filter_it, this->_joint_filters)
    {
        joint_filter_it.second->setJointId(_joint_id);
    }

    this->_joint_normalized_prediction_errors[RIGID_JOINT] = -1.;
    this->_joint_normalized_prediction_errors[PRISMATIC_JOINT] = -1.;
    this->_joint_normalized_prediction_errors[REVOLUTE_JOINT] = -1.;
    this->_joint_normalized_prediction_errors[DISCONNECTED_JOINT] = -1.;
}

void JointCombinedFilter::setJointLikelihoodDisconnected(double disconnected_joint_likelihood)
{
    boost::static_pointer_cast<DisconnectedJointFilter>(this->_joint_filters[DISCONNECTED_JOINT])->setLikelihoodOfLastMeasurements(disconnected_joint_likelihood);
}

void JointCombinedFilter::setLoopPeriodNS(double loop_period_ns)
{
    BOOST_FOREACH(joint_filters_map::value_type joint_filter_it, this->_joint_filters)
    {
        joint_filter_it.second->setLoopPeriodNS(loop_period_ns);
    }
}

void JointCombinedFilter::setNumSamplesForLikelihoodEstimation(int likelihood_sample_num)
{
    BOOST_FOREACH(joint_filters_map::value_type joint_filter_it, this->_joint_filters)
    {
        joint_filter_it.second->setNumSamplesForLikelihoodEstimation(likelihood_sample_num);
    }
}

void JointCombinedFilter::setNormalizingTerm(double normalizing_term)
{
    BOOST_FOREACH(joint_filters_map::value_type joint_filter_it, this->_joint_filters)
    {
        joint_filter_it.second->setNormalizingTerm(normalizing_term);
    }
}

void JointCombinedFilter::setCovarianceDeltaMeasurementLinear(double sigma_delta_meas_uncertainty_linear)
{
    boost::static_pointer_cast<PrismaticJointFilter>(this->_joint_filters[PRISMATIC_JOINT])->setCovarianceDeltaMeasurementLinear(sigma_delta_meas_uncertainty_linear);
}

void JointCombinedFilter::setCovarianceDeltaMeasurementAngular(double sigma_delta_meas_uncertainty_angular)
{
    boost::static_pointer_cast<RevoluteJointFilter>(this->_joint_filters[REVOLUTE_JOINT])->setCovarianceDeltaMeasurementAngular(sigma_delta_meas_uncertainty_angular);
}

void JointCombinedFilter::initialize()
{
    BOOST_FOREACH(joint_filters_map::value_type joint_filter_it, this->_joint_filters)
    {
        joint_filter_it.second->initialize();
    }
}

void JointCombinedFilter::setMaxTranslationRigid(double rig_max_translation)
{
    boost::static_pointer_cast<RigidJointFilter>(this->_joint_filters[RIGID_JOINT])->setMaxTranslationRigid(rig_max_translation);
}

void JointCombinedFilter::setMaxRotationRigid(double rig_max_rotation)
{
    boost::static_pointer_cast<RigidJointFilter>(this->_joint_filters[RIGID_JOINT])->setMaxRotationRigid(rig_max_rotation);
}

void JointCombinedFilter::setMinRotationRevolute(const double& value)
{
    boost::static_pointer_cast<RevoluteJointFilter>(this->_joint_filters[REVOLUTE_JOINT])->setMinRotationRevolute(value);
}

void JointCombinedFilter::setMaxRadiusDistanceRevolute(const double& value)
{
    boost::static_pointer_cast<RevoluteJointFilter>(this->_joint_filters[REVOLUTE_JOINT])->setMaxRadiusDistanceRevolute(value);
}

void JointCombinedFilter::setCovariancePrior(const JointFilterType& joint_type, double prior_cov_vel)
{
    this->_joint_filters[joint_type]->setCovariancePrior(prior_cov_vel);
}

void JointCombinedFilter::setCovarianceAdditiveSystemNoisePhi(const JointFilterType& joint_type, double sys_noise_phi)
{
    this->_joint_filters[joint_type]->setCovarianceAdditiveSystemNoisePhi(sys_noise_phi);
}

void JointCombinedFilter::setCovarianceAdditiveSystemNoiseTheta(const JointFilterType& joint_type, double sys_noise_theta)
{
    this->_joint_filters[joint_type]->setCovarianceAdditiveSystemNoiseTheta(sys_noise_theta);
}

void JointCombinedFilter::setCovarianceAdditiveSystemNoisePx(const JointFilterType& joint_type, double sys_noise_px)
{
    this->_joint_filters[joint_type]->setCovarianceAdditiveSystemNoisePx(sys_noise_px);
}

void JointCombinedFilter::setCovarianceAdditiveSystemNoisePy(const JointFilterType& joint_type, double sys_noise_py)
{
    this->_joint_filters[joint_type]->setCovarianceAdditiveSystemNoisePy(sys_noise_py);
}

void JointCombinedFilter::setCovarianceAdditiveSystemNoisePz(const JointFilterType& joint_type, double sys_noise_pz)
{
    this->_joint_filters[joint_type]->setCovarianceAdditiveSystemNoisePz(sys_noise_pz);
}

void JointCombinedFilter::setCovarianceAdditiveSystemNoiseJointState(const JointFilterType& joint_type, double sys_noise_js)
{
    this->_joint_filters[joint_type]->setCovarianceAdditiveSystemNoiseJointState(sys_noise_js);
}

void JointCombinedFilter::setCovarianceAdditiveSystemNoiseJointVelocity(const JointFilterType& joint_type, double sys_noise_jv)
{
    this->_joint_filters[joint_type]->setCovarianceAdditiveSystemNoiseJointVelocity(sys_noise_jv);
}

void JointCombinedFilter::setCovarianceMeasurementNoise(const JointFilterType& joint_type, double meas_noise)
{
    this->_joint_filters[joint_type]->setCovarianceMeasurementNoise(meas_noise);
}

void JointCombinedFilter::setInitialMeasurement(const joint_measurement_t &initial_measurement,
                                        const Eigen::Twistd& rrb_pose_at_srb_birth_in_sf,
                                                const Eigen::Twistd& srb_pose_at_srb_birth_in_sf)
{
    BOOST_FOREACH(joint_filters_map::value_type joint_filter_it, this->_joint_filters)
    {
        joint_filter_it.second->setInitialMeasurement(initial_measurement,
                                                      rrb_pose_at_srb_birth_in_sf,
                                                      srb_pose_at_srb_birth_in_sf);
    }
}

JointCombinedFilter::~JointCombinedFilter()
{

}

JointCombinedFilter::JointCombinedFilter(const JointCombinedFilter &joint)
{
    this->_joint_id = joint._joint_id;
    this->_joint_filters = joint._joint_filters;
    this->_joint_normalized_prediction_errors = joint._joint_normalized_prediction_errors;
    this->_normalizing_term = joint._normalizing_term;
}

void JointCombinedFilter::predictMeasurement()
{
    BOOST_FOREACH(joint_filters_map::value_type joint_filter_it, this->_joint_filters)
    {
        joint_filter_it.second->predictMeasurement();
    }
}

std::map<JointFilterType, Eigen::Twistd> JointCombinedFilter::getPredictedMeasurement()
{
    std::map<JointFilterType, Eigen::Twistd> second_rb_hypotheses;
    BOOST_FOREACH(joint_filters_map::value_type joint_filter_it, this->_joint_filters)
    {
        second_rb_hypotheses[joint_filter_it.first] = joint_filter_it.second->getPredictedMeasurement();
    }
    return second_rb_hypotheses;
}

void JointCombinedFilter::setMeasurement(joint_measurement_t acquired_measurement, const double &measurement_timestamp_ns)
{
    BOOST_FOREACH(joint_filters_map::value_type joint_filter_it, this->_joint_filters)
    {
        joint_filter_it.second->setMeasurement(acquired_measurement, measurement_timestamp_ns);
    }
}

void JointCombinedFilter::correctState()
{
    BOOST_FOREACH(joint_filters_map::value_type joint_filter_it, this->_joint_filters)
    {
        joint_filter_it.second->correctState();
    }
}

void JointCombinedFilter::predictState(double time_interval_ns)
{
    BOOST_FOREACH(joint_filters_map::value_type joint_filter_it, this->_joint_filters)
    {
        joint_filter_it.second->predictState(time_interval_ns);
    }
}

void JointCombinedFilter::normalizeJointFilterProbabilities()
{
    this->_normalizing_term = 0.0;
    double sum_of_unnormalized_model_probabilities = 0.0;

    // First we iterate over all the joints and collect their unnormalized likelihoods
    // Except the disconnected joint that is already normalized!
    BOOST_FOREACH(joint_filters_map::value_type joint_filter_it, this->_joint_filters)
    {
        //if(joint_filter_it.second->getJointFilterType() != DISCONNECTED_JOINT)
        //{
            sum_of_unnormalized_model_probabilities += joint_filter_it.second->getUnnormalizedProbabilityOfJointFilter();
        //}
    }

    double normalized_disconnected_probability = this->_joint_filters[DISCONNECTED_JOINT]->getProbabilityOfJointFilter();
    this->_normalizing_term =
            (sum_of_unnormalized_model_probabilities);// / (1.0 - normalized_disconnected_probability);

    // If the normalizing term is 0 the probabilities would become infinite!
    if(_normalizing_term < 1e-5)
    {
        _normalizing_term = 1e-5;
    }

    BOOST_FOREACH(joint_filters_map::value_type joint_filter_it, this->_joint_filters)
    {
        joint_filter_it.second->setNormalizingTerm(this->_normalizing_term);
    }
}

void JointCombinedFilter::estimateJointFilterProbabilities()
{
    BOOST_FOREACH(joint_filters_map::value_type joint_filter_it, this->_joint_filters)
    {
        joint_filter_it.second->estimateMeasurementHistoryLikelihood();
        joint_filter_it.second->estimateUnnormalizedModelProbability();
    }
    this->normalizeJointFilterProbabilities();
}

JointFilterPtr JointCombinedFilter::getMostProbableJointFilter()
{
    double max_probability_of_a_model = 0.0;
    JointFilterPtr most_probable_joint_type = this->_joint_filters.at(DISCONNECTED_JOINT);
    BOOST_FOREACH(joint_filters_map::value_type joint_filter_it, this->_joint_filters)
    {
        double probability_of_a_model = joint_filter_it.second->getProbabilityOfJointFilter();
        if (probability_of_a_model > max_probability_of_a_model)
        {
            max_probability_of_a_model = probability_of_a_model;
            most_probable_joint_type = this->_joint_filters.at(joint_filter_it.first);
        }
    }
    return most_probable_joint_type;
}

JointFilterPtr JointCombinedFilter::getJointFilter(JointFilterType joint_type) const
{
    return this->_joint_filters.at(joint_type);
}

JointCombinedFilterId JointCombinedFilter::getJointCombinedFilterId() const
{
    return this->_joint_id;
}

void JointCombinedFilter::setJointCombinedFilterId(JointCombinedFilterId new_joint_combined_filter_id)
{
    this->_joint_id = new_joint_combined_filter_id;
}

